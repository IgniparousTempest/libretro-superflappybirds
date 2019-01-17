#ifndef LR_SUPERFLAPPYBIRD_TEXTURES_H
#define LR_SUPERFLAPPYBIRD_TEXTURES_H

#include <SDL_render.h>

class Textures {
public:
    SDL_Texture* sky;
    SDL_Texture* buildings;
    SDL_Texture* ground;
    SDL_Texture* pipe_top;
    SDL_Texture* pipe_bottom;
    SDL_Texture* bird;

    int sky_w;
    int buildings_w;
    int ground_w;
    int pipe_top_w;
    int pipe_bottom_w;
    int bird_w;

    int sky_h;
    int buildings_h;
    int ground_h;
    int pipe_top_h;
    int pipe_bottom_h;
    int bird_h;

    explicit Textures(SDL_Renderer *renderer) {
        SDL_Surface *sky = SDL_LoadBMP("resources/sky.bmp");
        SDL_Surface *buildings = SDL_LoadBMP("resources/buildings.bmp");
        SDL_Surface *ground = SDL_LoadBMP("resources/ground.bmp");
        SDL_Surface *pipe_top = SDL_LoadBMP("resources/pipe_top.bmp");
        SDL_Surface *pipe_bottom = SDL_LoadBMP("resources/pipe_bottom.bmp");
        SDL_Surface *bird = SDL_LoadBMP("resources/bird.bmp");

        this->sky = SDL_CreateTextureFromSurface(renderer, sky);
        this->buildings = SDL_CreateTextureFromSurface(renderer, buildings);
        this->ground = SDL_CreateTextureFromSurface(renderer, ground);
        this->pipe_top = SDL_CreateTextureFromSurface(renderer, pipe_top);
        this->pipe_bottom = SDL_CreateTextureFromSurface(renderer, pipe_bottom);
        this->bird = SDL_CreateTextureFromSurface(renderer, bird);

        sky_w = sky->w;
        buildings_w = buildings->w;
        ground_w = ground->w;
        pipe_top_w = pipe_top->w;
        pipe_bottom_w = pipe_bottom->w;
        bird_w = bird->w;

        sky_h = sky->h;
        buildings_h = buildings->h;
        ground_h = ground->h;
        pipe_top_h = pipe_top->h;
        pipe_bottom_h = pipe_bottom->h;
        bird_h = bird->h;

        SDL_FreeSurface(sky);
        SDL_FreeSurface(buildings);
        SDL_FreeSurface(ground);
        SDL_FreeSurface(pipe_top);
        SDL_FreeSurface(pipe_bottom);
        SDL_FreeSurface(bird);
    }
};

#endif //LR_SUPERFLAPPYBIRD_TEXTURES_H
