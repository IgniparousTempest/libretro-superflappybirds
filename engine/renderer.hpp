#ifndef LR_SUPERFLAPPYBIRDS_RENDERERER_HPP
#define LR_SUPERFLAPPYBIRDS_RENDERERER_HPP

#include <iostream>
#include <cstdint>
#include <cassert>
#include <cmath>
#include "rect.hpp"
#include "omp.h"
#include "texture.hpp"

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

    /// Fill the screen with the specified colour.
    /// \param colour The colour to fill the screen with.
    void Clear(uint32_t colour) {
#pragma omp parallel for
        for (int x = 0; x <width; ++x) {
            for (int y = 0; y < height; ++y) {
                framebuffer[y * width + x] = colour;
            }
        }
    }

    /// Renders the whole image to the screen at the position and size defined in the rect.
    /// \param image The image to draw to the screen.
    /// \param dest The position and size to draw the image at.
    void Render(Texture* image, Rect* dest);

    /// Draws a part of an image to the screen.
    /// \param image The image to draw to the screen.
    /// \param src The subsection of the image to draw.
    /// \param dest The position and size to draw the image at.
    void Render(Texture* image, Rect* src, Rect* dest);

    /// Draws an image to the screen.
    /// \param image The image to draw to the screen.
    /// \param src The subsection of the image to draw.
    /// \param dest The position and size to draw the image at.
    /// \param angle The angle in degrees to rotate the image.
    void Render(Texture* image, Rect* src, Rect* dest, double angle);

    /// Renders the whole image to the screen at the position and size defined in the rect.
    /// The image's alpha is ignored and the specified opacity is used instead.
    /// \param image The image to draw to the screen.
    /// \param dest The position and size to draw the image at.
    /// \param opacity The opacity to draw the image at, should be between 0 and 1.
    void RenderForceAlpha(Texture* image, Rect* dest, double opacity);

private:
    /// Draws the addend pixel over the augend pixel with the specified transparency.
    /// \param augend The base pixel.
    /// \param addend The top pixel.
    /// \param opacity The opacity to draw the top pixel with.
    /// \return The result of the pixel addition.
    uint32_t OverlayPixels(uint32_t augend, uint32_t addend, double opacity);
};


#endif //LR_SUPERFLAPPYBIRDS_RENDERERER_HPP
