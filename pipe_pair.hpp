#ifndef LR_SUPERFLAPPYBIRD_PIPE_H
#define LR_SUPERFLAPPYBIRD_PIPE_H


#include <utility>
#include "textures.hpp"
#include "engine/renderer.hpp"

class PipePair {
public:
    int x;
    int y;
    int gap;

    PipePair(int x, int y, int gap);
    void Render(Renderer *renderer, Textures* textures, int distance_travelled);
    std::pair<Rect, Rect> GetRect(Textures* textures, int distance_travelled);
};


#endif //LR_SUPERFLAPPYBIRD_PIPE_H
