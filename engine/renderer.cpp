#include "renderer.hpp"

void Renderer::Render(Texture *image, Rect *dest) {
    // Skip this render call if the destination is off the screen.
    if (dest->x > width || dest->y > height || dest->x + dest->w < 0 || dest->y + dest->h < 0)
        return;

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

void Renderer::Render(Texture *image, Rect *src, Rect *dest) {
    // Skip this render call if the destination is off the screen.
    if (dest->x > width || dest->y > height || dest->x + dest->w < 0 || dest->y + dest->h < 0)
        return;

    double xs = src->w / (double) dest->w; // x-scale
    double ys = src->h / (double) dest->h; // y-scale
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
                pixel = image->image[(src->y + (int) (y * ys)) * image->w + (src->x + (int) (x * xs))];
                alpha = pixel >> 24;
                //TODO: This can only handle full alpha or no alpha
                if (alpha != 0)
                    framebuffer[screen_y * width + screen_x] = pixel;
            }
        }
    }
}

void Renderer::Render(Texture *image, Rect *src, Rect *dest, double angle) {
    // Skip this render call if the destination is off the screen.
    if (dest->x > width || dest->y > height || dest->x + dest->w < 0 || dest->y + dest->h < 0)
        return;

    assert(src->w == dest->w && src->h == dest->h);
    int hw = dest->w / 2;
    int hh = dest->h / 2;

#pragma omp parallel for
    for (int x = -hw; x < hw * 3; ++x) {
        int screen_x, screen_y, sx, sy, dest_index, alpha;
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
            screen_x = dest->x + x;
            screen_y = dest->y + y;
            if (screen_x >= 0 && screen_y >= 0 && screen_x < width && screen_y < height) {
                dest_index = screen_y * width + screen_x;
                pixel = image->image[(src->y + sy) * image->w + (src->x + sx)];
                alpha = pixel >> 24;
                //TODO: This can only handle full alpha or no alpha
                if (alpha != 0 && sx >= 0 && sy >= 0 && sx < src->w && sy < src->h)
                    framebuffer[dest_index] = pixel;
            }
        }
    }
}

void Renderer::RenderForceAlpha(Texture *image, Rect *dest, double opacity) {
    // Skip this render call if the destination is off the screen.
    if (dest->x > width || dest->y > height || dest->x + dest->w < 0 || dest->y + dest->h < 0)
        return;

    opacity = std::max(0.0, std::min(opacity, 1.0));

#pragma omp parallel for
    for (int x = 0; x < dest->w; ++x) {
        int screen_x;
        int screen_y;
        uint32_t pixel;
        uint32_t pixel_buffer;
        for (int y = 0; y < dest->h; ++y) {
            screen_x = dest->x + x;
            screen_y = dest->y + y;
            if (screen_x >= 0 && screen_y >= 0 && screen_x < width && screen_y < height) {
                pixel = image->image[y * dest->w + x];
                pixel_buffer = framebuffer[screen_y * width + screen_x];
                framebuffer[screen_y * width + screen_x] = OverlayPixels(pixel_buffer, pixel, opacity);
            }
        }
    }
}

uint32_t Renderer::OverlayPixels(uint32_t augend, uint32_t addend, double opacity) {
    if (addend >> 24 == 0)
        return augend;
    double opacity_invert = 1.0 - opacity;
    uint32_t pixel = 0;
    auto b = static_cast<uint8_t >(static_cast<uint8_t >(augend) * opacity_invert + static_cast<uint8_t >(addend) * opacity);
    auto g = static_cast<uint8_t >(static_cast<uint8_t >(augend >> 8) * opacity_invert + static_cast<uint8_t >(addend >> 8) * opacity);
    auto r = static_cast<uint8_t >(static_cast<uint8_t >(augend >> 16) * opacity_invert + static_cast<uint8_t >(addend >> 16) * opacity);
    pixel |= b;
    pixel |= g << 8;
    pixel |= r << 16;
    pixel |= 255 << 24;
    return pixel;
}
