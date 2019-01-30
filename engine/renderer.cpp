#include "renderer.hpp"

void Renderer::Render(Texture *image, Rect *dest) {
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

void Renderer::Render(Texture *image, Rect *src, Rect *dest, double angle) {
    assert(src->w == dest->w && src->h == dest->h);
    int hw = dest->w / 2;
    int hh = dest->h / 2;

#pragma omp parallel for
    for (int x = -hw; x < hw * 3; ++x) {
        int sx, sy, dest_index, alpha;
        double s, c, px, py;
        uint32_t pixel;
        for (int y = -hh; y < hh * 3; ++y) {
            s = std::sin(-angle * M_PI / 180);
            c = std::cos(-angle * M_PI / 180);
            px = x - hw;
            py = y - hh;
            // TODO: This is nearest-neighbour interpolation, use cubic instead. Once I support transparency better.
            sx = std::round(px * c - py * s + hw);
            sy = std::round(px * s + py * c + hh);
            dest_index = (dest->y + y) * width + (dest->x + x);
            pixel = image->image[(src->y + sy) * image->w + (src->x + sx)];
            alpha = pixel >> 24;
            //TODO: This can only handle full alpha or no alpha
            if (alpha != 0 && sx >= 0 && sy >= 0 && sx < src->w && sy < src->h)
                framebuffer[dest_index] = pixel;
        }
    }
}
