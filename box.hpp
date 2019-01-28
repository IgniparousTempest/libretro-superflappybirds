#ifndef LR_SUPERFLAPPYBIRDS_BOX_H
#define LR_SUPERFLAPPYBIRDS_BOX_H

#include <algorithm>
#include <vector>
#include <cstdint>
#include <cmath>

struct Pos
{
    Pos() : x(0), y(0) {}
    Pos(int x, int y) : x(x), y(y) {}

    Pos& operator+=(Pos pos)       { x += pos.x; y += pos.y; return *this; }
    Pos& operator-=(Pos pos)       { x -= pos.x; y -= pos.y; return *this; }
    Pos& operator*=(Pos pos)       { x *= pos.x; y *= pos.y; return *this; }
    Pos& operator/=(int div)       { x /= div; y /= div; return *this; }
    Pos  operator+ (Pos pos) const { return { x + pos.x, y + pos.y }; }
    Pos  operator- (Pos pos) const { return { x - pos.x, y - pos.y }; }
    Pos  operator* (Pos pos) const { return { x * pos.x, y * pos.y }; }
    Pos  operator/ (int div) const { return { x / div, y / div }; }
    bool operator==(Pos pos) const { return x == pos.x && y == pos.y; }
    bool operator!=(Pos pos) const { return !(*this == pos); }

    int x, y;
};

struct Rect
{
    Rect() : w(0), h(0) {}
    Rect(int x, int y, int w, int h) : x(x), y(y), w(w), h(h) {}

    Rect& operator+=(Pos pos)       { this->x += pos.x; this->y += pos.y; return *this; }
    Rect& operator-=(Pos pos)       { this->x -= pos.x; this->y -= pos.y; return *this; }
    Rect  operator+ (Pos pos) const { return { this->x + pos.x, this->y + pos.y, w, h }; }
    Rect  operator- (Pos pos) const { return { this->x - pos.x, this->y - pos.y, w, h }; }

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

class Screen {
public:
    int width;
    int height;
    uint32_t* framebuffer;

    Screen(int width, int height): width(width), height(height) {
        framebuffer = new uint32_t[width * height];
    }

    static uint32_t rgb(uint8_t r, uint8_t g, uint8_t b) {
        return r << 16 | g << 8 | b;
    }

    void Render(uint32_t* image, Rect* dest) {
#pragma omp parallel for
        for (int x = 0; x < dest->w; ++x) {
            int screen_x;
            int screen_y;
            for (int y = 0; y < dest->h; ++y) {
                screen_x = dest->x + x;
                screen_y = dest->y + y;
                framebuffer[screen_y * width + screen_x] = image[y * dest->w + x];
            }
        }
    }
};

class Box {
public:
    double x;
    double y;
    double angle;
    double speed = 20;
    uint32_t* texture;
    Rect* rect;

    Box(int x, int y, double angle) {
        this->angle = angle;
        texture = new uint32_t[20 * 40];
        for (int i = 0; i < 20*40; ++i) {
            texture[i] = Screen::rgb(0, 255, 0);
        }
        this->x = x;
        this->y = y;
        rect = new Rect(x, y, 20, 40);
    }

    void Update(double delta_time) {
        double moved = speed * delta_time;
        x += std::cos(angle);
        y += std::sin(angle);
        rect->x += (int)x;
        rect->y += (int)y;
    }

    void Render(Screen* screen) {
        screen->Render(texture, rect);
    }
};

#endif //LR_SUPERFLAPPYBIRDS_BOX_H
