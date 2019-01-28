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
        //TODO: Make this less complicated
        return (*this & *other) != Rect(0, 0, 0, 0);
    }

    // Intersection
    Rect  operator&(Rect rect) const
    {
        int x_left  = std::max(x, rect.x);
        int x_right = std::min(x + w, rect.x + rect.w);
        int width   = x_right - x_left;

        int y_top    = std::max(y, rect.y);
        int y_bottom = std::min(y + h, rect.y + rect.h);
        int height   = y_bottom - y_top;

        if (width <= 0 || height <= 0)
            return {0, 0, 0, 0};
        else
            return {x_left, y_top, width, height};
    }

    Rect& operator&=(Rect rect)
    {
        *this = operator&(rect);
        return *this;
    }

    explicit operator bool() const { return w > 0 && h > 0; }

    int x, y, w, h;
};

#endif //LR_SUPERFLAPPYBIRDS_RECT_HPP
