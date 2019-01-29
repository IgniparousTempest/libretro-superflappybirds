#ifndef LR_SUPERFLAPPYBIRDS_RENDERERER_HPP
#define LR_SUPERFLAPPYBIRDS_RENDERERER_HPP

#include <iostream>
#include <cstdint>
#include <cassert>
#include <cmath>
#include "rect.hpp"
#include "omp.h"

class Renderer {
public:
    int width;
    int height;
    uint32_t* framebuffer;

    Renderer(int width, int height): width(width), height(height) {
        framebuffer = new uint32_t[width * height];
    }

    static uint32_t rgb(uint8_t r, uint8_t g, uint8_t b) {
        return r << 16 | g << 8 | b;
    }

    void Clear(uint32_t colour) {
#pragma omp parallel for
        for (int x = 0; x <width; ++x) {
            for (int y = 0; y < height; ++y) {
                framebuffer[y * width + x] = colour;
            }
        }
    }

    void Render(Texture* image, Rect* dest) {
#pragma omp parallel for
        for (int x = 0; x < dest->w; ++x) {
            int screen_x;
            int screen_y;
            uint32_t pixel;
            int alpha;
            for (int y = 0; y < dest->h; ++y) {
                screen_x = dest->x + x;
                screen_y = dest->y + y;
                if (screen_x >= 0 && screen_y >= 0 && screen_x < width && screen_y < height) {
                    pixel = image->image[y * dest->w + x];
                    alpha = pixel >> 24;
                    //TODO: This can only handle full alpha or no alpha
                    if (alpha != 0)
                        framebuffer[screen_y * width + screen_x] = pixel;
                }
            }
        }
    }

    void Render(Texture* image, Rect* src, Rect* dest) {
        double xs = src->w / (double)dest->w; // x-scale
        double ys = src->h / (double)dest->h; // y-scale
#pragma omp parallel for
        for (int x = 0; x < dest->w; ++x) {
            int screen_x;
            int screen_y;
            uint32_t pixel;
            int alpha;
            for (int y = 0; y < dest->h; ++y) {
                screen_x = dest->x + x;
                screen_y = dest->y + y;
                if (screen_x >= 0 && screen_y >= 0 && screen_x < width && screen_y < height) {
                    pixel = image->image[(src->y + (int)(y * ys)) * image->w + (src->x + (int)(x * xs))];
                    alpha = pixel >> 24;
                    //TODO: This can only handle full alpha or no alpha
                    if (alpha != 0)
                        framebuffer[screen_y * width + screen_x] = pixel;
                }
            }
        }
    }

    void Render(Texture* image, Rect* src, Rect* dest, double angle) {
        assert(src->w == dest->w && src->h == dest->h)
        angle *= -M_PI / 180; // Convert to radians
        int hw = dest->w / 2;
        int hh = dest->h / 2;

#pragma omp parallel for
        for (int x = -hw; x < dest->w + hw; ++x) {
            double dx, dy;
            int screen_x, screen_y;
            uint32_t pixel;
            int alpha;
            for (int y = -hh; y < dest->h - hh; ++y) {
                std::vector<std::pair<int, int>> quad;
                dx = dest->x + (x-hw) * std::cos(angle) - (y-hh) * std::sin(angle) + hw;
                dy = dest->y + (x-hw) * std::sin(angle) + (y-hh) * std::cos(angle) + hh;
                quad.push_back({(int)dx, (int)dy});
                quad.push_back({(int)dx + 1, (int)dy});
                quad.push_back({(int)dx, (int)dy + 1});
                quad.push_back({(int)dx + 1, (int)dy + 1});
                for (int i = 0; i < quad.size(); ++i) {
                    if (quad[i].first > 0 && quad[i].second > 0 && quad[i].first < src->w && quad[i].second < src->h) {
                        screen_x = dest->x + quad[i].first;
                        screen_y = dest->y + quad[i].second;
                        pixel = image->image[quad[i].second * dest->w + quad[i].first];
                        alpha = pixel >> 24;
                        //TODO: This can only handle full alpha or no alpha
                        if (alpha != 0)
                            framebuffer[screen_y * width + screen_x] = pixel;
                    }
                }
            }
        }
    }
};


#endif //LR_SUPERFLAPPYBIRDS_RENDERERER_HPP
