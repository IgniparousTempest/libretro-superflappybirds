#include <SDL_surface.h>
#include <iostream>
#include <algorithm>
#include "game.h"
#include "image_library.h"
#include "auxillary.h"

Game::Game(unsigned int screen_width, unsigned int screen_height) {
    state = InMenu;
    this->screen_width = screen_width;
    this->screen_height = screen_height;
    framebuffer.resize(screen_width * screen_height);
    rng.seed(std::random_device()());

    surface = ImageLib::create_sdl_surface(screen_width, screen_height);
    renderer = SDL_CreateSoftwareRenderer(surface);
    textures = new Textures(renderer);

    menu = new Menu(textures->title, textures->credits, textures->start_1_player, textures->start_2_player, textures->start_3_player, textures->start_4_player, textures->hand, textures->winner_background);

//    NewGame(4);
}

uint32_t *Game::surface_to_framebuffer(SDL_Surface* surface) {
    int bpp = surface->format->BytesPerPixel;
    for (int x = 0; x < surface->w; ++x) {
        for (int y = 0; y < surface->h; ++y) {
            Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;
            if(SDL_BYTEORDER != SDL_BIG_ENDIAN)
                framebuffer[y * surface->w + x] = p[0] << 16 | p[1] << 8 | p[2];
            else
                framebuffer[y * surface->w + x] = p[0] | p[1] << 8 | p[2] << 16;
        }
    }

    return &framebuffer[0];
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

void Game::DrawBackground(SDL_Renderer *renderer) {
    SDL_Rect dest_rect;

    // Draw Sky
    for (int x = 0; x < screen_width / textures->sky_w + 1; ++x) {
        for (int y = 0; y < screen_height / textures->sky_h + 1; ++y) {
            dest_rect.x = x * textures->sky_w;
            dest_rect.y = y * textures->sky_h;
            dest_rect.w = textures->sky_w;
            dest_rect.h = textures->sky_h;
            SDL_RenderCopy(renderer, textures->sky, nullptr, &dest_rect);
        }
    }

    // Draw Buildings
    for (int x = 0; x < screen_width / textures->buildings_w + 1; ++x) {
        dest_rect.x = x * textures->buildings_w;
        dest_rect.y = screen_height - textures->buildings_h;
        dest_rect.w = textures->buildings_w;
        dest_rect.h = textures->buildings_h;
        SDL_RenderCopy(renderer, textures->buildings, nullptr, &dest_rect);
    }
}

void Game::DrawGround(SDL_Renderer *renderer) {
    SDL_Rect dest_rect;

    int x_offset = (int)distance_travelled % textures->ground_w;

    // Draw Ground
    for (int x = 0; x < screen_width / textures->ground_w + 2; ++x) {
        dest_rect.x = x * textures->ground_w - x_offset;
        dest_rect.y = screen_height - textures->ground_h;
        dest_rect.w = textures->ground_w;
        dest_rect.h = textures->ground_h;
        SDL_RenderCopy(renderer, textures->ground, nullptr, &dest_rect);
    }
}

void Game::draw_score(int x, int y, int score, SDL_Texture* bird, SDL_Rect* bird_frame) {
    // Draw background
    SDL_Rect dest_rect = {x, y, textures->score_background_w, textures->score_background_h};
    SDL_RenderCopy(renderer, textures->score_background, nullptr, &dest_rect);

    // Draw player's bird
    dest_rect = {x + 11, y + 14, bird_frame->w, bird_frame->h};
    SDL_RenderCopy(renderer, bird, bird_frame, &dest_rect);

    // Draw score
    auto rects = Auxillary::getNumberRects(score, &textures->numbers_frames, x + 35, y + 10, 1);
    for (auto &rect : rects)
        SDL_RenderCopy(renderer, textures->numbers, &rect.first, &rect.second);
}

void Game::DrawScores(SDL_Renderer *renderer) {
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
    DrawBackground(renderer);
    for (auto &pipe : pipes)
        pipe.Render(renderer, textures, (int)distance_travelled);
    for (auto bird : birds)
        bird->Render(renderer);
    DrawGround(renderer);
    if (state != InMenu)
        DrawScores(renderer);
    if (state == InPostGameMenu || state == InMenu)
        menu->Render(renderer);
    return surface_to_framebuffer(surface);
}

bool Game::bird_crashed(Bird *bird) {
    if (!bird->IsAlive())
        return false;

    SDL_Rect bird_rect = bird->GetRect();

    SDL_Rect ground_rect = {0, (int)screen_height - textures->ground_h, (int)screen_width, textures->ground_h};
    if (SDL_HasIntersection(&bird_rect, &ground_rect))
        return true;
    for (auto &pipe : pipes) {
        auto rects = pipe.GetRect(textures, (int)distance_travelled);
        if (SDL_HasIntersection(&bird_rect, &rects.first) || SDL_HasIntersection(&bird_rect, &rects.second))
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
    std::cout << "Starting a new game with " << num_players << "players." << std::endl;
    for (auto bird : birds)
        delete bird;
    birds = {};
    int floor_height = screen_height - textures->ground_h;
    birds.push_back(new Bird(screen_width / 2, screen_height / 2, floor_height, textures->bird, textures->bird_frames));
    if (num_players >= 2)
        birds.push_back(new Bird(screen_width / 2, screen_height / 2, floor_height, textures->bird2, textures->bird_frames));
    if (num_players >= 3)
        birds.push_back(new Bird(screen_width / 2, screen_height / 2, floor_height, textures->bird3, textures->bird_frames));
    if (num_players == 4)
        birds.push_back(new Bird(screen_width / 2, screen_height / 2, floor_height, textures->bird4, textures->bird_frames));

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
    std::vector<SDL_Texture *> textures;
    std::vector<SDL_Rect *> rects;
    for (auto bird : birds) {
        if (bird->score > highest_score)
            highest_score = bird->score;
        textures.push_back(bird->texture);
        rects.push_back(&bird->animation_frames[2]);
    }

    menu->ShowScore(textures, rects);
    std::cout << "Game over, entering post game score screen." << std::endl;
}

void Game::score_all_birds() {
    for (auto bird : birds)
        if (bird->IsAlive())
            bird->score = std::max(0, (int)((bird->x + distance_travelled - screen_width - textures->pipe_bottom_w) / DISTANCE_BETWEEN_PIPES));
}
