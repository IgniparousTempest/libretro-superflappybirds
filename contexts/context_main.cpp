#include <tuple>
#include "context_main.hpp"
#include "context_high_score_view.hpp"

ContextMain::ContextMain(GameManager *game_manager, AudioMixer *mixer, unsigned int screen_width, unsigned int screen_height,
                         Assets *assets, SaveData *save_data, unsigned int max_players,
                         bool show_wins) : Context(game_manager), assets(assets), save_data(save_data), mixer(mixer) {
    state = InMenu;
    this->screen_width = screen_width;
    this->screen_height = screen_height;
    rng.seed(std::random_device()());
    this->show_wins = show_wins;

    menu = new Menu(assets->title, assets->credits, assets->start_1_player, assets->start_2_player, assets->start_3_player, assets->start_4_player, assets->start_5_player, assets->start_6_player, assets->start_7_player, assets->start_8_player, assets->start_single_player, assets->button_new_high_Score, assets->arrow_left, assets->arrow_right, assets->hand, assets->winner_background, assets->numbers, assets->numbers_frames, max_players);
}

void ContextMain::Update(double delta_time, std::vector<Input> controller_inputs) {
    // Show high scores if game has been idle for a while.
    if (state == InMenu) {
        idle_timer += delta_time;
        if (idle_timer > 5) {
            gameManager->PushNewContext(new ContextHighScoreView(gameManager, assets, save_data));
            idle_timer = 0;
        }
    }

    if (state == InGame || state == InMenu)
        distance_travelled += scroll_speed;// * delta_time;

    if (state == InPostGameMenu || state == InMenu)
        menu->Update(delta_time);

    if (state == InGame && all_birds_dead()) {
        PostGameMenu();
    }

    for (auto bird : birds)
        bird->Update(delta_time, distance_travelled);

    if (state == InGame) {
        // Collision detection
        for (int i = 0; i < birds.size(); ++i)
            if (bird_crashed(birds[i])) {
                std::cout << "Player " << i << " crashed with score: " << birds[i]->score << std::endl;
                birds[i]->Kill(distance_travelled);
                mixer->Play(assets->snd_hit);
            }

        for (int i = 0; i < birds.size(); ++i) {
            if (controller_inputs[i].flap_pressed)
                birds[i]->Flap();
        }

        if (!pipes.empty() && pipes.front().x + assets->pipe_bottom->w < distance_travelled)
            pipes.pop_front();
        if (pipes.size() < 8)
            generate_pipes(20);

        score_all_birds();
    }
    else if (state == InPostGameMenu || state == InMenu) {
        unsigned long controllers = (birds.empty())? 1 : birds.size();
        for (int i = 0; i < controllers; ++i) {
            // Reset idle timer when a button is pressed
            if (controller_inputs[i].left_pressed || controller_inputs[i].right_pressed || controller_inputs[i].flap_pressed)
                idle_timer = 0;
            if (controller_inputs[i].left_pressed)
                menu->Left();
            if (controller_inputs[i].right_pressed)
                menu->Right();
            if (controller_inputs[i].flap_pressed) {
                int players;
                if (menu->Select(&players))
                    NewGame(players);
                else if (players == MENU_RESPONSE_SHOW_HIGH_SCORE_INPUT) {
                    gameManager->PushNewContext(contextHighScoreInput);
                    contextHighScoreInput = nullptr;
                }
            }
        }
    }
}

void ContextMain::Render(Renderer *renderer) {
    DrawBackground(renderer);
    for (auto &pipe : pipes)
        pipe.Render(renderer, assets, (int)distance_travelled);
    for (auto bird : birds)
        bird->Render(renderer);
    DrawGround(renderer);
    if (state != InMenu)
        DrawScores(renderer);
    if (state == InPostGameMenu || state == InMenu)
        menu->Render(renderer);
}

void ContextMain::generate_pipes(int number) {
    std::uniform_int_distribution<std::mt19937::result_type> dist6((unsigned long)(screen_height / 3), (unsigned long)(2 * screen_height / 3));

    const int gap_between_top_and_bottom = 60;
    int last_x = 0;
    if (pipes.empty())
        last_x = (int)distance_travelled + screen_width;
    else
        last_x = pipes.back().x;

    for (int i = 0; i < number; ++i) {
        last_x += DISTANCE_BETWEEN_PIPES;
        pipes.emplace_back(last_x, dist6(rng), gap_between_top_and_bottom);
    }
}

void ContextMain::DrawBackground(Renderer *renderer) {
    Rect dest_rect;

    // Draw Sky
    renderer->Clear(Renderer::rgb(115, 183, 196));

    // Draw Buildings
    for (int x = 0; x < screen_width / assets->buildings->w + 1; ++x) {
        dest_rect.x = x * assets->buildings->w;
        dest_rect.y = screen_height - assets->buildings->h;
        dest_rect.w = assets->buildings->w;
        dest_rect.h = assets->buildings->h;
        renderer->Render(assets->buildings, &dest_rect);
    }
}

void ContextMain::DrawGround(Renderer *renderer) {
    Rect dest_rect;

    int x_offset = (int)distance_travelled % assets->ground->w;

    // Draw Ground
    for (int x = 0; x < screen_width / assets->ground->w + 2; ++x) {
        dest_rect.x = x * assets->ground->w - x_offset;
        dest_rect.y = screen_height - assets->ground->h;
        dest_rect.w = assets->ground->w;
        dest_rect.h = assets->ground->h;
        renderer->Render(assets->ground, &dest_rect);
    }
}

void ContextMain::draw_score(Renderer* renderer, int x, int y, int score) {
    // Draw score
    auto rects = Auxillary::getNumberRects(score, &assets->numbers_frames, x, y, 1);
    auto x_offset = (rects.back().second.x - rects.front().second.x + rects.back().second.w) / 2;
    for (auto &rect : rects)
        rect.second.x -= x_offset;
    for (auto &rect : rects)
        renderer->Render(assets->numbers, &rect.first, &rect.second);
}

void ContextMain::draw_score(Renderer* renderer, int x, int y, int score, Texture* bird, Rect* bird_frame, int wins) {
    Texture *background;
    // Choose background with or without score
    if (wins >= 0)
        background = assets->score_wins_background;
    else
        background = assets->score_background;

    // Draw background
    Rect dest_rect = {x, y, background->w, background->h};
    renderer->Render(background, &dest_rect);

    // Draw player's bird
    dest_rect = {x + 11, y + 14, bird_frame->w, bird_frame->h};
    renderer->Render(bird, bird_frame, &dest_rect);

    // Draw score
    auto rects = Auxillary::getNumberRects(score, &assets->numbers_frames, x + 35, y + 10, 1);
    for (auto &rect : rects)
        renderer->Render(assets->numbers, &rect.first, &rect.second);

    // Draw wins
    if (wins >= 0) {
        rects = Auxillary::getNumberRects(wins, &assets->numbers_frames, x + 35, y + 32, 1, 0.5f);
        for (auto &rect : rects)
            renderer->Render(assets->numbers, &rect.first, &rect.second);
    }
}

void ContextMain::DrawScores(Renderer *renderer) {
    if (birds.size() == 1) {
        draw_score(renderer, screen_width / 2, 20, birds[0]->score);
    } else {
        int back_w = assets->score_wins_background->w;
        int back_h = assets->score_wins_background->h;
        std::vector<unsigned int> xs = {0, screen_width / 3 - back_w / 2, 2 * screen_width / 3 - back_w / 2, screen_width - back_w};
        Bird *bird;
        int wins = -1;
        for (int i = 0; i < birds.size(); ++i) {
            bird = birds[i];
            if (show_wins)
                wins = bird->wins;
            draw_score(renderer, xs[i % 4], i >= 4 ? screen_height - back_h : 0, bird->score, bird->animation->texture, &bird->animation->frames[0], wins);
        }
    }
}

bool ContextMain::bird_crashed(Bird *bird) {
    if (!bird->IsAlive())
        return false;

    Rect bird_rect = bird->GetRect();

    Rect ground_rect = {0, (int)screen_height - assets->ground->h, (int)screen_width, assets->ground->h};
    if (bird_rect.HasIntersection(&ground_rect))
        return true;
    for (auto &pipe : pipes) {
        auto rects = pipe.GetRect(assets, (int)distance_travelled);
        if (bird_rect.HasIntersection(&rects.first) || bird_rect.HasIntersection(&rects.second))
            return true;
    }

    return false;
}

bool ContextMain::all_birds_dead() {
    for (auto bird : birds)
        if (bird->IsAlive())
            return false;
    return true;
}

void ContextMain::NewGame(int num_players) {
    state = InGame;
    std::cout << "Setting up a new game with " << num_players << " players." << std::endl;
    std::vector<int> wins;
    std::vector<std::string> names;
    for (auto bird : birds) {
        wins.push_back(bird->wins);
        names.push_back(bird->name);
        delete bird;
    }
    birds = {};
    std::vector<Texture*> textures = {assets->bird, assets->bird2, assets->bird3, assets->bird4, assets->bird5, assets->bird6, assets->bird7, assets->bird8};
    int floor_height = screen_height - assets->ground->h;
    for (int i = 0; i < num_players; ++i) {
        int x_offset = -20 * (num_players / 2 + 1) + 20 * (num_players - i);
        birds.push_back(new Bird(screen_width / 2 + x_offset, screen_height / 2, floor_height, textures[i], assets->bird_frames));
        // Set the bird's number of wins
        if (num_players == wins.size()) {
            birds.back()->wins = wins[i];
            birds.back()->name = names[i];
        }
    }

    distance_travelled = 0;
    pipes = {};
    std::cout << "Starting game." << std::endl;
}

void ContextMain::PostGameMenu() {
    state = InPostGameMenu;

    // Check if any players broke into the high score table
    std::vector<std::pair<Bird*, int>> player_tuple;
    for (int i = 0; i < birds.size(); ++i)
        player_tuple.emplace_back(birds[i], i + 1);
    std::sort(player_tuple.begin(), player_tuple.end(), [](auto &left, auto &right) {
        return left.first->score > right.first->score;
    });

    std::vector<int> scores;
    for (auto &item : player_tuple)
        scores.push_back(item.first->score);

    int high_scorers = save_data->DoesPlayerQualifyForHighScoreTable(scores);
    std::vector<Bird*> player_birds;
    std::vector<int> player_numbers;
    if (high_scorers > 0) {
        // put players in player order, not in score order.
        for (int i = 0; i < birds.size(); ++i) {
            for (int j = 0; j < high_scorers; ++j) {
                if (i + 1 == player_tuple[j].second) {
                    player_birds.push_back(player_tuple[j].first);
                    player_numbers.push_back(i + 1);
                }
            }
        }
        menu->ShowNewHighScoreButton();
        contextHighScoreInput = new ContextHighScoreInput(gameManager, assets, save_data, player_birds, player_numbers);
    }

    // Get winners, i.e. the player or players who got the top score
    int highest_score = player_tuple[0].first->score;
    std::vector<Texture *> textures;
    std::vector<Rect *> rects;
    for (auto bird : player_tuple) {
        if (bird.first->score == highest_score) {
            textures.push_back(bird.first->animation->texture);
            rects.push_back(&bird.first->animation->frames[0]);
            bird.first->wins += 1;
        }
    }

    menu->ShowScore(highest_score, save_data->HighScore(), textures, rects);
    std::cout << "Game over, entering post game score screen." << std::endl;
}

void ContextMain::score_all_birds() {
    int new_Score;
    for (auto bird : birds)
        if (bird->IsAlive()) {
            new_Score = std::max(0, (int) ((bird->x + distance_travelled - screen_width - assets->pipe_bottom->w) /
                                           DISTANCE_BETWEEN_PIPES));
            if (bird->score != new_Score)
                mixer->Play(assets->snd_score);
            bird->score = new_Score;
        }
}
