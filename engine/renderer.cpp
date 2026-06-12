#include "renderer.hpp"
#include <algorithm>

void Renderer::Render(Texture *image, Rect *dest) {
    if (dest->x >= width || dest->y >= height || dest->x + dest->w <= 0 || dest->y + dest->h <= 0)
        return;

    // Pre-clip dest against the framebuffer so the inner loop has no bounds checks.
    int x0 = std::max(0, dest->x);
    int y0 = std::max(0, dest->y);
    int x1 = std::min(width,  dest->x + dest->w);
    int y1 = std::min(height, dest->y + dest->h);
    int src_x0 = x0 - dest->x;
    int src_y0 = y0 - dest->y;

    for (int y = y0; y < y1; ++y) {
        const uint32_t* src_row = image->image + (src_y0 + (y - y0)) * dest->w + src_x0;
        uint32_t* dst_row = framebuffer + y * width + x0;
        int run = x1 - x0;
        for (int i = 0; i < run; ++i) {
            uint32_t pixel = src_row[i];
            // 1-bit alpha: any non-zero alpha byte means "opaque".
            if (pixel & 0xff000000u)
                dst_row[i] = pixel;
        }
    }
}

void Renderer::Render(Texture *image, Rect *src, Rect *dest) {
    if (dest->x >= width || dest->y >= height || dest->x + dest->w <= 0 || dest->y + dest->h <= 0)
        return;

    int x0 = std::max(0, dest->x);
    int y0 = std::max(0, dest->y);
    int x1 = std::min(width,  dest->x + dest->w);
    int y1 = std::min(height, dest->y + dest->h);

    // Use 16.16 fixed-point steppers to avoid double->int every pixel.
    int xs_q16 = (int)((src->w / (double)dest->w) * 65536.0);
    int ys_q16 = (int)((src->h / (double)dest->h) * 65536.0);
    int sx0_q16 = (x0 - dest->x) * xs_q16;
    int sy0_q16 = (y0 - dest->y) * ys_q16;

    int sy_q16 = sy0_q16;
    for (int y = y0; y < y1; ++y, sy_q16 += ys_q16) {
        const uint32_t* src_row = image->image + (src->y + (sy_q16 >> 16)) * image->w + src->x;
        uint32_t* dst_row = framebuffer + y * width + x0;
        int sx_q16 = sx0_q16;
        int run = x1 - x0;
        for (int i = 0; i < run; ++i, sx_q16 += xs_q16) {
            uint32_t pixel = src_row[sx_q16 >> 16];
            if (pixel & 0xff000000u)
                dst_row[i] = pixel;
        }
    }
}

void Renderer::Render(Texture *image, Rect *src, Rect *dest, double angle) {
    if (dest->x >= width || dest->y >= height || dest->x + dest->w <= 0 || dest->y + dest->h <= 0)
        return;

    assert(src->w == dest->w && src->h == dest->h);
    int hw = dest->w / 2;
    int hh = dest->h / 2;

    // Hoist trig — depends only on `angle`.
    double s = std::sin(-angle * M_PI / 180.0);
    double c = std::cos(-angle * M_PI / 180.0);

    // Iterate the same expanded window the original used, but pre-clipped against the screen.
    int dst_x_lo = dest->x + (-hw);
    int dst_x_hi = dest->x + (hw * 3);
    int dst_y_lo = dest->y + (-hh);
    int dst_y_hi = dest->y + (hh * 3);
    int sx_lo = std::max(0, dst_x_lo);
    int sy_lo = std::max(0, dst_y_lo);
    int sx_hi = std::min(width,  dst_x_hi);
    int sy_hi = std::min(height, dst_y_hi);

    for (int screen_y = sy_lo; screen_y < sy_hi; ++screen_y) {
        int y = screen_y - dest->y;
        double py = y - hh;
        uint32_t* dst_row = framebuffer + screen_y * width;
        for (int screen_x = sx_lo; screen_x < sx_hi; ++screen_x) {
            int x = screen_x - dest->x;
            double px = x - hw;
            int sx = (int)std::round(px * c - py * s + hw);
            int sy = (int)std::round(px * s + py * c + hh);
            if (sx >= 0 && sy >= 0 && sx < src->w && sy < src->h) {
                uint32_t pixel = image->image[(src->y + sy) * image->w + (src->x + sx)];
                if (pixel & 0xff000000u)
                    dst_row[screen_x] = pixel;
            }
        }
    }
}

void Renderer::RenderForceAlpha(Texture *image, Rect *dest, double opacity) {
    if (dest->x >= width || dest->y >= height || dest->x + dest->w <= 0 || dest->y + dest->h <= 0)
        return;

    opacity = std::max(0.0, std::min(opacity, 1.0));

    int x0 = std::max(0, dest->x);
    int y0 = std::max(0, dest->y);
    int x1 = std::min(width,  dest->x + dest->w);
    int y1 = std::min(height, dest->y + dest->h);
    int src_x0 = x0 - dest->x;
    int src_y0 = y0 - dest->y;

    for (int y = y0; y < y1; ++y) {
        const uint32_t* src_row = image->image + (src_y0 + (y - y0)) * dest->w + src_x0;
        uint32_t* dst_row = framebuffer + y * width + x0;
        int run = x1 - x0;
        for (int i = 0; i < run; ++i) {
            dst_row[i] = OverlayPixels(dst_row[i], src_row[i], opacity);
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
