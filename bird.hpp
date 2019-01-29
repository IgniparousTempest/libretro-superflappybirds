#ifndef LR_SUPERFLAPPYBIRD_BIRD_H
#define LR_SUPERFLAPPYBIRD_BIRD_H


#include "assets.hpp"
#include "engine/renderer.hpp"

enum BirdState { Gliding, Alive, Dead };

class Bird {
public:
    int x;
    int y;
    int score = 0;
    Texture* texture;
    std::vector<Rect> animation_frames;
    Bird(int x, int y, int floor_height, Texture* texture, std::vector<Rect> frames);
    void Update(double delta_time, double distance_travelled);
    void Flap();
    void Kill(double distance_travelled);
    void Render(Renderer* renderer);
    Rect GetRect();
    bool IsAlive();

private:
    int floor_height;
    int x_died;
    double distance_died;
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
