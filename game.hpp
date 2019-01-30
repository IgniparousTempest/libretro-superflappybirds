#ifndef LR_SUPERFLAPPYBIRD_GAME_H
#define LR_SUPERFLAPPYBIRD_GAME_H

#include <stdint.h>
#include <random>
#include <vector>
#include <deque>
#include "pipe_pair.hpp"
#include "assets.hpp"
#include "bird.hpp"
#include "input.hpp"
#include "menu.hpp"
#include "settings.hpp"

enum GameState { InGame, InPostGameMenu, InMenu };

class Game {
public:
    const char* game_name = "Super Flappy Birds";
    const char* game_version = "0.9.0";
    const int game_fps = 50;

    Game(unsigned int screen_width, unsigned int screen_height, std::string path);
    void GameLoop(double delta_time, std::vector<Input> controller_inputs);
    uint32_t* GetFrameBuffer();

private:
    /// The speed in pixels per second that the screen scrolls at
    double_t scroll_speed = 1;
    /// How many pixels between each pipe
    const int DISTANCE_BETWEEN_PIPES = 100;

    Renderer* screen;
    std::mt19937 rng;
    GameState state;
    Menu* menu;
    Settings* settings;
    double distance_travelled = 0;
    Assets* assets;
    std::vector<uint32_t> framebuffer;
    unsigned int screen_width;
    unsigned int screen_height;
    std::deque<PipePair> pipes = {};
    std::vector<Bird*> birds;

    void DrawBackground(Renderer* renderer);
    void DrawGround(Renderer* renderer);
    void DrawScores(Renderer *renderer);
    void generate_pipes(int number);
    bool bird_crashed(Bird* bird);
    bool all_birds_dead();
    void score_all_birds();
    void draw_score(Renderer* renderer, int x, int y, int score, Texture* bird, Rect* bird_frame);
    void NewGame(int num_players);
    /// Transitions the game to the Post Game Menu
    void PostGameMenu();
};


#endif //LR_SUPERFLAPPYBIRD_GAME_H
