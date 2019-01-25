#ifndef LR_SUPERFLAPPYBIRD_BIRD_H
#define LR_SUPERFLAPPYBIRD_BIRD_H


#include <SDL_render.h>
#include "textures.h"

enum BirdState { Alive, Dead, Menu };

class Bird {
public:
    int x = 0;
    int y = 0;
    int score = 0;
    BirdState state;
    Bird(int x, SDL_Texture* texture, int frame_width, int frame_height);
    void Update(double delta_time);
    void Flap();
    void Kill();
    void Render(SDL_Renderer* renderer);
    SDL_Rect GetRect();

private:
    SDL_Texture* texture;
    int frame_width;
    int frame_height;
    double speed_y = 0;
    const double FLAP_SPEED = -3.5;
    const double GRAVITY = 0.25;
    const double TERMINAL_VELOCITY = 5;
    unsigned int frames = 0;
    const int frames_in_animation = 4;
    const int frames_per_animation_frame = 3;
};


#endif //LR_SUPERFLAPPYBIRD_BIRD_H
