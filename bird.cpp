#include <iostream>
#include "bird.h"

Bird::Bird(int x, int y, int floor_height, SDL_Texture* texture, std::vector<SDL_Rect> frames) {
    this->x = x;
    this->y = y;
    this->floor_height = floor_height;
    this->texture = texture;
    animation_frames = frames;
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
}

void Bird::Update(double delta_time, double distance_travelled) {
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
            auto f = animation_frames[0];
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

void Bird::Render(SDL_Renderer *renderer) {
    frames += 1;
    int frame_index = (frames / frames_per_animation_frame) % frames_in_animation;
    if (state == Dead)
        frame_index = 0;
    SDL_Rect dest_rect;

    dest_rect = GetRect();

    //TODO: Remove the ternary. It is a solution on my dev machine to stop textures from glitching.
    SDL_Rect* src_rect = &animation_frames[frame_index + ((frame_index == 0) ? 2 : 0)];

    double angle = (state == Dead)? 90 : speed_y * 10.0;
    SDL_RenderCopyEx(renderer, texture, src_rect, &dest_rect, angle, nullptr, SDL_FLIP_NONE);
}

SDL_Rect Bird::GetRect() {
    auto f = animation_frames[0];
    return {x - f.w / 2, y - f.h / 2, f.w, f.h};
}

bool Bird::IsAlive() {
    return state == Alive || state == Gliding;
}
