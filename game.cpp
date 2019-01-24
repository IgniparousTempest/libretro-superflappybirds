#include <SDL_surface.h>
#include <iostream>
#include <algorithm>
#include "game.h"
#include "image_library.h"

Game::Game(unsigned int screen_width, unsigned int screen_height) {
    this->screen_width = screen_width;
    this->screen_height = screen_height;
    framebuffer.resize(screen_width * screen_height);
    rng.seed(std::random_device()());

    surface = ImageLib::create_sdl_surface(screen_width, screen_height);
    renderer = SDL_CreateSoftwareRenderer(surface);
    textures = new Textures(renderer);

    birds.push_back(new Bird(screen_width / 2, textures->bird, textures->bird_w / 4, textures->bird_h));
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
        last_x = screen_width;
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

void Game::GameLoop(double delta_time, std::vector<Input> controller_inputs) {
    distance_travelled += scroll_speed;// * delta_time;
    for (auto bird : birds)
        bird->Update(delta_time);

    // Collision detection
    for (auto bird : birds)
        if (bird_crashed(bird))
            std::cout << "Crashed" << std::endl;

    for (int i = 0; i < birds.size(); ++i) {
        if (controller_inputs[i].flap_pressed)
            birds[i]->Flap();
    }

    if (!pipes.empty() && pipes.front().x + textures->pipe_bottom_w < distance_travelled)
        pipes.pop_front();
    if (pipes.size() < 5)
        generate_pipes(20);
}

uint32_t* Game::GetFrameBuffer() {
    DrawBackground(renderer);
    for (auto &pipe : pipes)
        pipe.Render(renderer, textures, (int)distance_travelled);
    DrawGround(renderer);
    for (auto bird : birds)
        bird->Render(renderer);
    return surface_to_framebuffer(surface);
}

bool Game::bird_crashed(Bird *bird) {
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
