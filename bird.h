#ifndef LR_SUPERFLAPPYBIRD_BIRD_H
#define LR_SUPERFLAPPYBIRD_BIRD_H


#include <SDL_render.h>
#include "textures.h"

class Bird {
public:
    int x = 0;
    int y = 0;
    explicit Bird(int x);
    void Update();
    void Render(SDL_Renderer* renderer, Textures* textures);

private:
    double speed_y = 0;
    const double GRAVITY = 0.5;
    const double TERMINAL_VELOCITY = 5.0;
    int frames = 0;
    const int frames_in_animation = 4;
    const int frames_per_animation_frame = 3;
};


#endif //LR_SUPERFLAPPYBIRD_BIRD_H
