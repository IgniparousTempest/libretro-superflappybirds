#include <iostream>
#include "bird.h"

Bird::Bird(int x, SDL_Texture* texture, int frame_width, int frame_height) {
    this->x = x;
    this->texture = texture;
    this->frame_width = frame_width;
    this->frame_height = frame_height;
    state = Alive;
}

void Bird::Flap() {
    if (state == Alive)
        speed_y = FLAP_SPEED;
}

void Bird::Kill() {
    state = Dead;
}

void Bird::Update(double delta_time) {
    switch (state) {
        case Alive:
            speed_y += GRAVITY;// * delta_time;
            if (speed_y > TERMINAL_VELOCITY)
                speed_y = TERMINAL_VELOCITY;
            y += (int) speed_y;
            break;
        case Dead:
            x -= 1;
            break;
    }
}

void Bird::Render(SDL_Renderer *renderer) {
    frames += 1;
    int frame_offset = (frames / frames_per_animation_frame) % frames_in_animation;
    if (state == Dead)
        frame_offset = 0;
    SDL_Rect dest_rect, src_rect;

    dest_rect = GetRect();

    src_rect.x = frame_width * (frame_offset + ((frame_offset == 0) ? 2 : 0)); //TODO: Remove the ternary. It is a solution on my dev machine to stop textures from glitching.
    src_rect.y = 0;
    src_rect.w = dest_rect.w;
    src_rect.h = dest_rect.h;

    SDL_RenderCopyEx(renderer, texture, &src_rect, &dest_rect, speed_y * 10.0, nullptr, SDL_FLIP_NONE);
}

SDL_Rect Bird::GetRect() {
    return {x - frame_width / 2, y - frame_height / 2, frame_width, frame_height};
}
