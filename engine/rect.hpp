#ifndef LR_SUPERFLAPPYBIRDS_RECT_HPP
#define LR_SUPERFLAPPYBIRDS_RECT_HPP

#include <algorithm>
#include "pos.hpp"

struct Rect
{
    Rect() : w(0), h(0) {}
    Rect(int x, int y, int w, int h) : x(x), y(y), w(w), h(h) {}

    Rect& operator+=(Pos pos)       { this->x += pos.x; this->y += pos.y; return *this; }
    Rect& operator-=(Pos pos)       { this->x -= pos.x; this->y -= pos.y; return *this; }
    Rect  operator+ (Pos pos) const { return { this->x + pos.x, this->y + pos.y, w, h }; }
    Rect  operator- (Pos pos) const { return { this->x - pos.x, this->y - pos.y, w, h }; }
    bool  operator== (Rect rect) const { return this->x == rect.x && this->y == rect.y && this->w == rect.w && this->h == rect.h; }
    bool  operator!= (Rect rect) const { return !(*this == rect); }

    bool HasIntersection(Rect* other) {
        return !(x + w <= other->x || y + h <= other->y || x >= other->x + other->w || y >= other->y + other->h);
    }

    int x, y, w, h;
};

#endif //LR_SUPERFLAPPYBIRDS_RECT_HPP
