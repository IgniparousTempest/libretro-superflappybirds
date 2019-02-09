#include <iostream>
#include "bird.hpp"

Bird::Bird(int x, int y, int floor_height, Texture* texture, std::vector<Rect> frames) {
    this->x = x;
    this->y = y;
    this->floor_height = floor_height;
    animation = new Animation(texture, std::move(frames), FRAMES_PER_ANIMATION_FRAME);
    state = Gliding;
}

void Bird::Flap() {
    if (state == Gliding)
        state = Alive;
    if (state == Alive)
        speed_y = FLAP_SPEED;
}

void Bird::Kill(double distance_travelled) {
    speed_y = 0;
    state = Dead;
    x_died = x;
    distance_died = distance_travelled;
    animation->Pause();
    animation->frame_index = 0;
}

void Bird::Update(double delta_time, double distance_travelled) {
    animation->Update();

    if (state != Gliding) {
        speed_y += GRAVITY;// * delta_time;
        if (speed_y > TERMINAL_VELOCITY)
            speed_y = TERMINAL_VELOCITY;
        y += (int) speed_y;
    }

    switch (state) {
        case Alive:
        {
            // If bird is above screen
            auto f = animation->frames[0];
            if (y - f.h / 2 < 0) {
                y = f.h / 2;
                speed_y = 0;
            }
            break;
        }
        case Dead:
            x = x_died - (int)(distance_travelled - distance_died);
            if (y > floor_height)
                y = floor_height;
            break;
    }
}

void Bird::Render(Renderer *renderer) {
    Rect dest_rect = GetRect();

    double angle = (state == Dead)? 90 : speed_y * 10.0;
    renderer->Render(animation->texture, animation->CurrentFrame(), &dest_rect, angle);
}

Rect Bird::GetRect() {
    auto f = animation->frames[0];
    return {x - f.w / 2, y - f.h / 2, f.w, f.h};
}

bool Bird::IsAlive() {
    return state == Alive || state == Gliding;
}

Bird::~Bird() {
    delete animation;
}