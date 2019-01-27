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
#include "menu.h"

enum GameState { InGame, InPostGameMenu, InMenu };

class Game {
public:
    const char* game_name = "Super Flappy Birds";
    const char* game_version = "0.9.0";
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
    GameState state;
    Menu* menu;
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
    void DrawScores(SDL_Renderer *renderer);
    uint32_t* surface_to_framebuffer(SDL_Surface* surface);
    void generate_pipes(int number);
    bool bird_crashed(Bird* bird);
    bool all_birds_dead();
    void score_all_birds();
    void draw_score(int x, int y, int score, SDL_Texture* bird, SDL_Rect* bird_frame);
    void NewGame(int num_players);
    /// Transitions the game to the Post Game Menu
    void PostGameMenu();
};


#endif //LR_SUPERFLAPPYBIRD_GAME_H
