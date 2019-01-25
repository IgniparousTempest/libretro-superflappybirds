#ifndef LR_SUPERFLAPPYBIRD_BIRD_H
#define LR_SUPERFLAPPYBIRD_BIRD_H


#include <SDL_render.h>
#include "textures.h"

enum BirdState { Gliding, Alive, Dead };

class Bird {
public:
    int x;
    int y;
    int score = 0;
    SDL_Texture* texture;
    std::vector<SDL_Rect> animation_frames;
    Bird(int x, int y, SDL_Texture* texture, std::vector<SDL_Rect> frames);
    void Update(double delta_time);
    void Flap();
    void Kill();
    void Render(SDL_Renderer* renderer);
    SDL_Rect GetRect();
    bool IsAlive();

private:
    BirdState state;
    double speed_y = 0;
    const double FLAP_SPEED = -3.5;
    const double GRAVITY = 0.25;
    const double TERMINAL_VELOCITY = 5;
    unsigned int frames = 0;
    const int frames_in_animation = 4;
    const int frames_per_animation_frame = 3;
};


#endif //LR_SUPERFLAPPYBIRD_BIRD_H
