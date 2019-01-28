#include "pipe_pair.hpp"
#include "image_library.hpp"

PipePair::PipePair(int x, int y, int gap) {
    this->x = x;
    this->y = y;
    this->gap = gap;
}

void PipePair::Render(Renderer *renderer, Textures* textures, int distance_travelled) {
    auto rects = GetRect(textures, distance_travelled);

    // Bottom Pipe
    renderer->Render(textures->pipe_bottom, &rects.first);

    // Top Pipe
    renderer->Render(textures->pipe_top, &rects.second);
}

std::pair<Rect, Rect> PipePair::GetRect(Textures* textures, int distance_travelled) {
    Rect bot_rect, top_rect;
    // Bottom Pipe
    bot_rect.x = x - distance_travelled;
    bot_rect.y = y;
    bot_rect.w = textures->pipe_bottom_w;
    bot_rect.h = textures->pipe_bottom_h;

    // Top Pipe
    top_rect.x = x - distance_travelled;
    top_rect.y = y - gap - textures->pipe_top_h;
    top_rect.w = textures->pipe_top_w;
    top_rect.h = textures->pipe_top_h;
    return {bot_rect, top_rect};
}
