#include "renderer.hpp"

void Renderer::Render(Texture* image, Rect* src, Rect* dest) {
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