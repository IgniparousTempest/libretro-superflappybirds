#include "pipe_pair.h"
#include "image_library.h"

PipePair::PipePair(int x, int y, int gap) {
    this->x = x;
    this->y = y;
    this->gap = gap;
}

void PipePair::Render(SDL_Renderer *renderer, Textures* textures, int distance_travelled) {
    SDL_Rect dest_rect;
    // Bottom Pipe
    dest_rect.x = x - distance_travelled;
    dest_rect.y = y;
    dest_rect.w = textures->pipe_bottom_w;
    dest_rect.h = textures->pipe_bottom_h;
    SDL_RenderCopy(renderer, textures->pipe_bottom, nullptr, &dest_rect);

    // Top Pipe
    dest_rect.x = x - distance_travelled;
    dest_rect.y = y - gap - textures->pipe_top_h;
    dest_rect.w = textures->pipe_top_w;
    dest_rect.h = textures->pipe_top_h;
    SDL_RenderCopy(renderer, textures->pipe_top, nullptr, &dest_rect);
}
