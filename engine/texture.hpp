#ifndef LR_SUPERFLAPPYBIRDS_TEXTURE_HPP
#define LR_SUPERFLAPPYBIRDS_TEXTURE_HPP

#include <cstdint>

struct Texture {
public:
    uint32_t* image;
    int w, h;
    Texture(uint32_t* image, int w, int h): image(image), w(w), h(h) {}
};

#endif //LR_SUPERFLAPPYBIRDS_TEXTURE_HPP
