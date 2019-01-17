#ifndef LR_SUPERFLAPPYBIRD_IMAGE_LIBRARY_H
#define LR_SUPERFLAPPYBIRD_IMAGE_LIBRARY_H

#include <SDL_surface.h>

class ImageLib {
public:
    static SDL_Surface *create_sdl_surface(int width, int height) {
        Uint32 rmask, gmask, bmask, amask;
        rmask = 0x000000ff;
        gmask = 0x0000ff00;
        bmask = 0x00ff0000;
        amask = 0xff000000;

        return SDL_CreateRGBSurface(0, width, height, 32, rmask, gmask, bmask, amask);
    }
};

#endif //LR_SUPERFLAPPYBIRD_IMAGE_LIBRARY_H
