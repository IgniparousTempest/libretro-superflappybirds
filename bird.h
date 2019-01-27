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
    Bird(int x, int y, int floor_height, SDL_Texture* texture, std::vector<SDL_Rect> frames);
    void Update(double delta_time, double distance_travelled);
    void Flap();
    void Kill(double distance_travelled);
    void Render(SDL_Renderer* renderer);
    SDL_Rect GetRect();
    bool IsAlive();

private:
    int floor_height;
    int x_died;
    double distance_died;
    BirdState state;
    double speed_y = 0;
    const double FLAP_SPEED = -3.5;
    const double GRAVITY = 0.25;
    const double TERMINAL_VELOCITY = 2;
    double time = 0;
    const int frames_in_animation = 4;
    const double frames_per_second = 16;
};


#endif //LR_SUPERFLAPPYBIRD_BIRD_H
