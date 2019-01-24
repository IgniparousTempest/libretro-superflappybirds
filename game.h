#ifndef LR_SUPERFLAPPYBIRD_GAME_H
#define LR_SUPERFLAPPYBIRD_GAME_H

#include <stdint.h>
#include <random>
#include <vector>
#include <SDL_render.h>
#include <deque>
#include "pipe_pair.h"
#include "textures.h"
#include "bird.h"
#include "input.h"


class Game {
public:
    const char* game_name = "Super Flappy Birds";
    const char* game_version = "0.1.0";
    const int game_fps = 50;

    Game(unsigned int screen_width, unsigned int screen_height);
    void GameLoop(double delta_time, std::vector<Input> controller_inputs);
    uint32_t* GetFrameBuffer();

private:
    /// The speed in pixels per second that the screen scrolls at
    double_t scroll_speed = 1;
    /// How many pixels between each pipe
    const int DISTANCE_BETWEEN_PIPES = 100;

    std::mt19937 rng;
    double_t distance_travelled = 0;
    SDL_Renderer *renderer;
    SDL_Surface *surface;
    Textures* textures;
    std::vector<uint32_t> framebuffer;
    unsigned int screen_width;
    unsigned int screen_height;
    std::deque<PipePair> pipes = {};
    std::vector<Bird*> birds;

    void DrawBackground(SDL_Renderer* renderer);
    void DrawGround(SDL_Renderer* renderer);
    uint32_t* surface_to_framebuffer(SDL_Surface* surface);
    void generate_pipes(int number);
    bool bird_crashed(Bird* bird);
};


#endif //LR_SUPERFLAPPYBIRD_GAME_H
