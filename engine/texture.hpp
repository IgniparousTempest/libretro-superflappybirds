#ifndef LR_SUPERFLAPPYBIRDS_TEXTURE_HPP
#define LR_SUPERFLAPPYBIRDS_TEXTURE_HPP

#include <cstdint>

struct Texture {
public:
    uint32_t* image;
    int w, h;
    Texture(uint32_t* image, int w, int h): image(image), w(w), h(h) {}
    /// Copy Constructor
    Texture(const Texture& other): w(other.w), h(other.h) {
        image = new uint32_t[other.w * other.h];
        std::copy(other.image , other.image + other.w * other.h, image);
    }
    /// Copy Assignment Operator
    Texture& operator=(const Texture& other)
    {
        if (&other != this) {
            delete image;
            image = nullptr;
            image = new uint32_t[*(other.image)];
            std::copy(other.image , other.image + other.w * other.h, image);
            w = other.w;
            h = other.h;
        }
        return *this;
    }
    /// Destructor
    ~Texture() {delete image;}

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
