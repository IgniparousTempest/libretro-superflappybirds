#ifndef LR_SUPERFLAPPYBIRD_PIPE_H
#define LR_SUPERFLAPPYBIRD_PIPE_H


#include <SDL_surface.h>
#include <SDL_render.h>
#include "textures.h"

class PipePair {
public:
    int x;
    int y;
    int gap;

    PipePair(int x, int y, int gap);
    void Render(SDL_Renderer *renderer, Textures* textures, int distance_travelled);
};


#endif //LR_SUPERFLAPPYBIRD_PIPE_H
