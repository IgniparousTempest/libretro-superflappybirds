#ifndef LR_SUPERFLAPPYBIRDS_TEXTURE_HPP
#define LR_SUPERFLAPPYBIRDS_TEXTURE_HPP

#include <cstdint>

struct Texture {
public:
    uint32_t* image;
    int w, h;
    Texture(uint32_t* image, int w, int h): image(image), w(w), h(h) {}

    Texture* FlipHorizontally() {
        auto tex = new uint32_t[w * h];
        for (int x = 0; x < w; ++x) {
#pragma omp parallel for
            for (int y = 0; y < h; ++y) {
                tex[(h - y - 1) * w + x] = image[y * w + x];
            }
        }
        return new Texture(tex, w, h);
    }

    Texture* FlipVertical() {
        auto tex = new uint32_t[w * h];
        for (int x = 0; x < w; ++x) {
#pragma omp parallel for
            for (int y = 0; y < h; ++y) {
                tex[y * w + (w - x - 1)] = image[y * w + x];
            }
        }
        return new Texture(tex, w, h);
    }
};

#endif //LR_SUPERFLAPPYBIRDS_TEXTURE_HPP
