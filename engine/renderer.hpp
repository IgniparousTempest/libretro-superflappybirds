#ifndef LR_SUPERFLAPPYBIRDS_RENDERERER_HPP
#define LR_SUPERFLAPPYBIRDS_RENDERERER_HPP

#include <iostream>
#include <cstdint>
#include <cassert>
#include <cmath>
#include "rect.hpp"
#include "omp.h"
#include "CImg.hpp"

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
        assert(src->w == dest->w && src->h == dest->h);

        CImg<float> img(src->w, src->h, 1, 4);
        // Translate Texture to CImg
#pragma omp parallel for
        for (int x = 0; x < dest->w; ++x) {
            for (int y = 0; y < dest->h; ++y) {
                img(x, y, 0, 0) = image->image[y * dest->w + x] & 0xFF;
                img(x, y, 0, 1) = (image->image[y * dest->w + x] >> 8) & 0xFF;
                img(x, y, 0, 2) = (image->image[y * dest->w + x] >> 16) & 0xFF;
                img(x, y, 0, 3) = (image->image[y * dest->w + x] >> 24) & 0xFF;
            }
        }
        // Translate back
        uint32_t* data = new uint32_t[img.width() * img.height()];
#pragma omp parallel for
        for (int x = 0; x < dest->w; ++x) {
            for (int y = 0; y < dest->h; ++y) {
                data[y * dest->w + x] = buffer[y * surface->w + x] = img(x, y, 0, 0) | img(x, y, 0, 1) << 8 | img(x, y, 0, 2) << 16 | img(x, y, 0, 3) << 24;
            }
        }
    }
};


#endif //LR_SUPERFLAPPYBIRDS_RENDERERER_HPP
