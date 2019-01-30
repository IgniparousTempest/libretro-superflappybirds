#include <iostream>
#include <algorithm>
#include "game.hpp"
#include "auxillary.hpp"

Game::Game(unsigned int screen_width, unsigned int screen_height, std::string path) {
    state = InMenu;
    this->screen_width = screen_width;
    this->screen_height = screen_height;
    framebuffer.resize(screen_width * screen_height);
    rng.seed(std::random_device()());

    assets = new Assets(path);

    menu = new Menu(assets->title, assets->credits, assets->start_1_player, assets->start_2_player, assets->start_3_player, assets->start_4_player, assets->hand, assets->winner_background, assets->numbers, assets->numbers_frames);
    settings = new Settings();
    settings->Deserialize();

    screen = new Renderer(screen_width, screen_height);
}

void Game::generate_pipes(int number) {
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

void Game::DrawBackground(Renderer *renderer) {
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

void Game::DrawGround(Renderer *renderer) {
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

void Game::draw_score(Renderer* renderer, int x, int y, int score, Texture* bird, Rect* bird_frame) {
    // Draw background
    Rect dest_rect = {x, y, assets->score_background->w, assets->score_background->h};
    renderer->Render(assets->score_background, &dest_rect);

    // Draw player's bird
    dest_rect = {x + 11, y + 14, bird_frame->w, bird_frame->h};
    renderer->Render(bird, bird_frame, &dest_rect);

    // Draw score
    auto rects = Auxillary::getNumberRects(score, &assets->numbers_frames, x + 35, y + 10, 1);
    for (auto &rect : rects)
        renderer->Render(assets->numbers, &rect.first, &rect.second);
}

void Game::DrawScores(Renderer *renderer) {
    draw_score(renderer, 0, 0, birds[0]->score, birds[0]->texture, &birds[0]->animation_frames[2]);
    if (birds.size() >= 2)
        draw_score(renderer, screen_width / 3 - assets->score_background->w / 2, 0, birds[1]->score, birds[1]->texture, &birds[1]->animation_frames[2]);
    if (birds.size() >= 3)
        draw_score(renderer, 2 * screen_width / 3 - assets->score_background->w / 2, 0, birds[2]->score, birds[2]->texture, &birds[2]->animation_frames[2]);
    if (birds.size() >= 4)
        draw_score(renderer, screen_width - assets->score_background->w, 0, birds[3]->score, birds[3]->texture, &birds[3]->animation_frames[2]);
}

void Game::GameLoop(double delta_time, std::vector<Input> controller_inputs) {
    if (state == InGame || state == InMenu)
        distance_travelled += scroll_speed;// * delta_time;

    if (state == InGame && all_birds_dead()) {
        PostGameMenu();
    }

    for (auto bird : birds)
        bird->Update(delta_time, distance_travelled);

    if (state == InGame) {
        // Collision detection
        for (auto bird : birds)
            if (bird_crashed(bird)) {
                std::cout << "Crashed with score: " << bird->score << std::endl;
                bird->Kill(distance_travelled);
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
            if (controller_inputs[i].left_pressed)
                menu->Left();
            if (controller_inputs[i].right_pressed)
                menu->Right();
            if (controller_inputs[i].flap_pressed)
                NewGame(menu->Select());
        }
    }
}

uint32_t* Game::GetFrameBuffer() {
    DrawBackground(screen);
    for (auto &pipe : pipes)
        pipe.Render(screen, assets, (int)distance_travelled);
    for (auto bird : birds)
        bird->Render(screen);
    DrawGround(screen);
    if (state != InMenu)
        DrawScores(screen);
    if (state == InPostGameMenu || state == InMenu)
        menu->Render(screen);
    return screen->framebuffer;
}

bool Game::bird_crashed(Bird *bird) {
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

bool Game::all_birds_dead() {
    for (auto bird : birds)
        if (bird->IsAlive())
            return false;
    return true;
}

void Game::NewGame(int num_players) {
    state = InGame;
    std::cout << "Starting a new game with " << num_players << " players." << std::endl;
    for (auto bird : birds)
        delete bird;
    birds = {};
    int floor_height = screen_height - assets->ground->h;
    birds.push_back(new Bird(screen_width / 2, screen_height / 2, floor_height, assets->bird, assets->bird_frames));
    if (num_players >= 2)
        birds.push_back(new Bird(screen_width / 2 - 20, screen_height / 2, floor_height, assets->bird2, assets->bird_frames));
    if (num_players >= 3)
        birds.push_back(new Bird(screen_width / 2 + 20, screen_height / 2, floor_height, assets->bird3, assets->bird_frames));
    if (num_players == 4)
        birds.push_back(new Bird(screen_width / 2 - 40, screen_height / 2, floor_height, assets->bird4, assets->bird_frames));

    distance_travelled = 0;
    pipes = {};
}

void Game::PostGameMenu() {
    state = InPostGameMenu;

    // Get winners
    int highest_score = 0;
    for (auto bird : birds) {
        if (bird->score > highest_score)
            highest_score = bird->score;
    }
    std::vector<Texture *> textures;
    std::vector<Rect *> rects;
    for (auto bird : birds) {
        if (bird->score == highest_score) {
            textures.push_back(bird->texture);
            rects.push_back(&bird->animation_frames[2]);
        }
    }

    // Update highscore
    if (highest_score > settings->highscore) {
        settings->highscore = highest_score;
        settings->Serialize();
    }

    menu->ShowScore(highest_score, settings->highscore, textures, rects);
    std::cout << "Game over, entering post game score screen." << std::endl;
}

void Game::score_all_birds() {
    for (auto bird : birds)
        if (bird->IsAlive())
            bird->score = std::max(0, (int)((bird->x + distance_travelled - screen_width - assets->pipe_bottom->w) / DISTANCE_BETWEEN_PIPES));
}
