#include <iostream>
#include <algorithm>
#include "game.h"
#include "image_library.hpp"
#include "auxillary.hpp"

Game::Game(unsigned int screen_width, unsigned int screen_height) {
    state = InMenu;
    this->screen_width = screen_width;
    this->screen_height = screen_height;
    framebuffer.resize(screen_width * screen_height);
    rng.seed(std::random_device()());

    textures = new Textures();

    menu = new Menu(textures->title, textures->credits, textures->start_1_player, textures->start_2_player, textures->start_3_player, textures->start_4_player, textures->hand, textures->winner_background, textures->numbers, textures->numbers_frames);
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
//    for (int x = 0; x < screen_width / textures->sky->w + 1; ++x) {
//        for (int y = 0; y < screen_height / textures->sky->h + 1; ++y) {
//            dest_rect.x = x * textures->sky->w;
//            dest_rect.y = y * textures->sky->h;
//            dest_rect.w = textures->sky->w;
//            dest_rect.h = textures->sky->h;
//            renderer->Render(textures->sky, &dest_rect);
//        }
//    }

    // Draw Buildings
    for (int x = 0; x < screen_width / textures->buildings->w + 1; ++x) {
        dest_rect.x = x * textures->buildings->w;
        dest_rect.y = screen_height - textures->buildings->h;
        dest_rect.w = textures->buildings->w;
        dest_rect.h = textures->buildings->h;
        renderer->Render(textures->buildings, &dest_rect);
    }
}

void Game::DrawGround(Renderer *renderer) {
    Rect dest_rect;

    int x_offset = (int)distance_travelled % textures->ground->w;

    // Draw Ground
    for (int x = 0; x < screen_width / textures->ground->w + 2; ++x) {
        dest_rect.x = x * textures->ground->w - x_offset;
        dest_rect.y = screen_height - textures->ground->h;
        dest_rect.w = textures->ground->w;
        dest_rect.h = textures->ground->h;
        renderer->Render(textures->ground, &dest_rect);
    }
}

void Game::draw_score(int x, int y, int score, Texture* bird, Rect* bird_frame) {
    // Draw background
    Rect dest_rect = {x, y, textures->score_background_w, textures->score_background_h};
    screen->Render(textures->score_background, &dest_rect);

    // Draw player's bird
    dest_rect = {x + 11, y + 14, bird_frame->w, bird_frame->h};
    screen->Render(bird, bird_frame, &dest_rect);

    // Draw score
    auto rects = Auxillary::getNumberRects(score, &textures->numbers_frames, x + 35, y + 10, 1);
    for (auto &rect : rects)
        screen->Render(textures->numbers, &rect.first, &rect.second);
}

void Game::DrawScores(Renderer *renderer) {
    draw_score(0, 0, birds[0]->score, birds[0]->texture, &birds[0]->animation_frames[2]);
    if (birds.size() >= 2)
        draw_score(screen_width / 3 - textures->score_background_w / 2, 0, birds[1]->score, birds[1]->texture, &birds[1]->animation_frames[2]);
    if (birds.size() >= 3)
        draw_score(2 * screen_width / 3 - textures->score_background_w / 2, 0, birds[2]->score, birds[2]->texture, &birds[2]->animation_frames[2]);
    if (birds.size() >= 4)
        draw_score(screen_width - textures->score_background_w, 0, birds[3]->score, birds[3]->texture, &birds[3]->animation_frames[2]);
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

        if (!pipes.empty() && pipes.front().x + textures->pipe_bottom_w < distance_travelled)
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
        pipe.Render(screen, textures, (int)distance_travelled);
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

    Rect ground_rect = {0, (int)screen_height - textures->ground->h, (int)screen_width, textures->ground->h};
    if (bird_rect.HasIntersection(&ground_rect))
        return true;
    for (auto &pipe : pipes) {
        auto rects = pipe.GetRect(textures, (int)distance_travelled);
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
    int floor_height = screen_height - textures->ground->h;
    birds.push_back(new Bird(screen_width / 2, screen_height / 2, floor_height, textures->bird, textures->bird_frames));
    if (num_players >= 2)
        birds.push_back(new Bird(screen_width / 2 - 20, screen_height / 2, floor_height, textures->bird2, textures->bird_frames));
    if (num_players >= 3)
        birds.push_back(new Bird(screen_width / 2 + 20, screen_height / 2, floor_height, textures->bird3, textures->bird_frames));
    if (num_players == 4)
        birds.push_back(new Bird(screen_width / 2 - 40, screen_height / 2, floor_height, textures->bird4, textures->bird_frames));

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
            bird->score = std::max(0, (int)((bird->x + distance_travelled - screen_width - textures->pipe_bottom_w) / DISTANCE_BETWEEN_PIPES));
}
