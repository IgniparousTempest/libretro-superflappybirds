#include "bird.h"

Bird::Bird(int x) {
    this->x = x;
}

void Bird::Update() {
    speed_y += GRAVITY;
    if (speed_y > TERMINAL_VELOCITY)
        speed_y = TERMINAL_VELOCITY;
    y += (int)speed_y;
}

void Bird::Render(SDL_Renderer *renderer, Textures *textures) {
    Update();
    frames += 1;
    int frame_offset = frames / frames_per_animation_frame;
    if (frame_offset >= frames_in_animation) {
        frames = 0;
        frame_offset = 0;
    }
    SDL_Rect dest_rect, src_rect;

    dest_rect.x = x - textures->bird_w / frames_in_animation / 2;
    dest_rect.y = y - textures->bird_h / 2;
    dest_rect.w = textures->bird_w / frames_in_animation;
    dest_rect.h = textures->bird_h;

    src_rect.x = textures->bird_w / frames_in_animation * frame_offset;
    src_rect.y = 0;
    src_rect.w = textures->bird_w / frames_in_animation;
    src_rect.h = textures->bird_h;

    SDL_RenderCopy(renderer, textures->bird, &src_rect, &dest_rect);
}
