#ifndef LR_SUPERFLAPPYBIRD_BIRD_H
#define LR_SUPERFLAPPYBIRD_BIRD_H


#include "assets.hpp"
#include "engine/animation.hpp"
#include "engine/renderer.hpp"

enum BirdState { Gliding, Alive, Dead };

class Bird {
public:
    int x;
    int y;
    int score = 0;
    int wins = 0;
    std::string name = "";
    Animation* animation;
    static const int FRAMES_PER_ANIMATION_FRAME = 3;
    Bird(int x, int y, int floor_height, Texture* texture, std::vector<Rect> frames);
    ~Bird();
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
};


#endif //LR_SUPERFLAPPYBIRD_BIRD_H
