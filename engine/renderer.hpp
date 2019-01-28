#include <cstdint>
#include "rect.hpp"

class Renderer {
public:
    int width;
    int height;
    int buffer_length;
    uint32_t* framebuffer;

    Renderer(int width, int height): width(width), height(height) {
        buffer_length = width * height;
        framebuffer = new uint32_t[buffer_length];
    }

    static uint32_t rgb(uint8_t r, uint8_t g, uint8_t b) {
        return r << 16 | g << 8 | b;
    }

    void Render(uint32_t* image, Rect* dest) {
#pragma omp parallel for
        for (int x = 0; x < dest->w; ++x) {
            int screen_x;
            int screen_y;
            int pos;
            for (int y = 0; y < dest->h; ++y) {
                screen_x = dest->x + x;
                screen_y = dest->y + y;
                pos = screen_y * width + screen_x;
                if (pos < buffer_length)
                    framebuffer[pos] = image[y * dest->w + x];
            }
        }
    }
};