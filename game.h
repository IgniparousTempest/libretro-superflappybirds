#ifndef LR_SUPERFLAPPYBIRD_GAME_H
#define LR_SUPERFLAPPYBIRD_GAME_H

#include <stdint.h>
#include <random>
#include <list>
#include <vector>
#include <SDL_render.h>
#include "pipe_pair.h"
#include "textures.h"
#include "bird.h"


class Game {
public:
    const char* game_name = "SuperFlappyBird";
    const char* game_version = "1.0.0";

    Game(unsigned int screen_width, unsigned int screen_height);
    void GameLoop();
    uint32_t* GetFrameBuffer();

private:
    std::mt19937 rng;
    double_t distance_travelled = 0;
    /// The speed in pixels that the screen scrolls at
    double_t scroll_speed = 1.0;
    SDL_Renderer *renderer;
    SDL_Surface *surface;
    Textures* textures;
    std::vector<uint32_t> framebuffer;
    unsigned int screen_width;
    unsigned int screen_height;
    std::list<PipePair> pipes = {};
    Bird* bird;

    void DrawBackground(SDL_Renderer* renderer);
    void DrawGround(SDL_Renderer* renderer);
    uint32_t* surface_to_framebuffer(SDL_Surface* surface);
    void generate_pipes(int number);
};


#endif //LR_SUPERFLAPPYBIRD_GAME_H
