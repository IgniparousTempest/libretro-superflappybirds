#ifndef LR_SUPERFLAPPYBIRDS_BOX_H
#define LR_SUPERFLAPPYBIRDS_BOX_H

#include <SDL_render.h>

class Box {
public:
    double x;
    double y;
    double angle;
    double speed = 20;
    SDL_Texture* texture;
    SDL_Rect* rect;

    Box(int x, int y, double angle, SDL_Renderer* renderer) {
        this->angle = angle;
        SDL_Surface *sky = SDL_LoadBMP("resources/sky.bmp");
        texture = SDL_CreateTextureFromSurface(renderer, sky);
        this->x = x;
        this->y = y;
        rect = new SDL_Rect({x, y, sky->w, sky->h});
        SDL_FreeSurface(sky);
    }

    void Update(double delta_time) {
        double moved = speed * delta_time;
        x += std::cos(angle);
        y += std::sin(angle);
        rect->x += (int)x;
        rect->y += (int)y;
    }

    void Render(SDL_Renderer* renderer) {
        SDL_RenderCopy(renderer, texture, nullptr, rect);
    }
};

#endif //LR_SUPERFLAPPYBIRDS_BOX_H
