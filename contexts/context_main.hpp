#ifndef SUPERFLAPPYBIRDS_LIBRETRO_CONTEXT_MAIN_HPP
#define SUPERFLAPPYBIRDS_LIBRETRO_CONTEXT_MAIN_HPP


#include <random>
#include "context.hpp"
#include "../menu.hpp"
#include "../pipe_pair.hpp"
#include "../bird.hpp"
#include "context_highscore_input.hpp"

enum GameState {
    InGame, InPostGameMenu, InMenu
};

class ContextMain : public Context {
public:
    ContextMain(GameManager *game_manager, AudioMixer *mixer, unsigned int screen_width, unsigned int screen_height, Assets *assets, SaveData *save_data, unsigned int max_players, bool show_wins);

    void Update(double delta_time, std::vector<Input> controller_inputs) override;

    void Render(Renderer *renderer) override;

private:
    /// The speed in pixels per second that the screen scrolls at
    double_t scroll_speed = 1;
    /// How many pixels between each pipe
    const int DISTANCE_BETWEEN_PIPES = 100;

    AudioMixer *mixer;
    std::mt19937 rng;
    GameState state;
    Menu *menu;
    SaveData *save_data;
    double distance_travelled = 0;
    Assets *assets;
    unsigned int screen_width;
    unsigned int screen_height;
    std::deque<PipePair> pipes = {};
    std::vector<Bird *> birds;
    bool show_wins;
    double idle_timer = 0;
    ContextHighScoreInput *contextHighScoreInput = nullptr;

    void DrawBackground(Renderer *renderer);

    void DrawGround(Renderer *renderer);

    void DrawScores(Renderer *renderer);

    void generate_pipes(int number);

    bool bird_crashed(Bird *bird);

    bool all_birds_dead();

    void score_all_birds();

    void draw_score(Renderer *renderer, int x, int y, int score);

    /// Draws the players score, their sprite, a background, and optionally their number of wins.
    /// \param renderer The renderer to draw with.
    /// \param x The x position for the score background's top-left corner.
    /// \param y The y position for the score background's top-left corner.
    /// \param score The player's score.
    /// \param bird The texture for the player.
    /// \param bird_frame The frame in that texture to draw.
    /// \param wins The number of wins the player has had, use a negative number to not show scores and have a more compact background.
    void draw_score(Renderer *renderer, int x, int y, int score, Texture *bird, Rect *bird_frame, int wins = -1);

    void NewGame(int num_players);

    /// Transitions the game to the Post Game Menu
    void PostGameMenu();

};


#endif //SUPERFLAPPYBIRDS_LIBRETRO_CONTEXT_MAIN_HPP
