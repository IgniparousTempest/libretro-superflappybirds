#include "pipe_pair.hpp"
#include "image_library.hpp"

PipePair::PipePair(int x, int y, int gap) {
    this->x = x;
    this->y = y;
    this->gap = gap;
}

void PipePair::Render(Renderer *renderer, Assets* assets, int distance_travelled) {
    auto rects = GetRect(assets, distance_travelled);

    // Bottom Pipe
    renderer->Render(assets->pipe_bottom, &rects.first);

    // Top Pipe
    renderer->Render(assets->pipe_top, &rects.second);
}

std::pair<Rect, Rect> PipePair::GetRect(Assets* assets, int distance_travelled) {
    Rect bot_rect, top_rect;
    // Bottom Pipe
    bot_rect.x = x - distance_travelled;
    bot_rect.y = y;
    bot_rect.w = assets->pipe_bottom->w;
    bot_rect.h = assets->pipe_bottom->h;

    // Top Pipe
    top_rect.x = x - distance_travelled;
    top_rect.y = y - gap - assets->pipe_top->h;
    top_rect.w = assets->pipe_top->w;
    top_rect.h = assets->pipe_top->h;
    return {bot_rect, top_rect};
}
