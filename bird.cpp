#include <iostream>
#include "bird.h"

Bird::Bird(int x, int y, SDL_Texture* texture, std::vector<SDL_Rect> frames) {
    this->x = x;
    this->y = y;
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

void Bird::Kill() {
    state = Dead;
}

void Bird::Update(double delta_time) {
    switch (state) {
        case Alive:
        {
            speed_y += GRAVITY;// * delta_time;
            if (speed_y > TERMINAL_VELOCITY)
                speed_y = TERMINAL_VELOCITY;
            y += (int) speed_y;

            // If bird is above screen
            auto f = animation_frames[0];
            if (y - f.h / 2 < 0) {
                y = f.h / 2;
                speed_y = 0;
            }
            break;
        }
        case Dead:
            x -= 1;
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

    SDL_RenderCopyEx(renderer, texture, src_rect, &dest_rect, speed_y * 10.0, nullptr, SDL_FLIP_NONE);
}

SDL_Rect Bird::GetRect() {
    auto f = animation_frames[0];
    return {x - f.w / 2, y - f.h / 2, f.w, f.h};
}

bool Bird::IsAlive() {
    return state == Alive || state == Gliding;
}
