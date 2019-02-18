#ifndef LR_SUPERFLAPPYBIRDS_MENU_H
#define LR_SUPERFLAPPYBIRDS_MENU_H


#include <vector>
#include "engine/texture.hpp"
#include "engine/rect.hpp"
#include "engine/renderer.hpp"

enum MenuResponse {
    MENU_RESPONSE_SHOW_HIGH_SCORE_INPUT = -100
};

class Menu {
public:
    Menu(Texture *texture_title, Texture *texture_credits, Texture *texture_start_1_player,
         Texture *texture_start_2_player, Texture *texture_start_3_player, Texture *texture_start_4_player,
         Texture *texture_start_5_player, Texture *texture_start_6_player, Texture *texture_start_7_player,
         Texture *texture_start_8_player, Texture *texture_start_single_player, Texture *new_highscore,
         Texture *texture_arrow_left, Texture *texture_arrow_right, Texture *texture_hand,
         Texture *texture_winner_background, Texture *texture_numbers, std::vector<Rect> numbers_frames,
         unsigned int max_players);

    void Left();

    void Right();

    /// Performs a select action on the menu and might return the number of players for the next game.
    /// \param number_of_players The number of players for the next game. The value is not altered if the next game can't be started.
    /// \return True if a new game should be started, false otherwise.
    bool Select(int *number_of_players);

    /// Changes from showing the main menu to showing the score screen.
    /// \param score The score for the winner.
    /// \param highscore The highest score achieved.
    /// \param texture_bird The textures for all the winning players.
    /// \param frame_rect The src rects for all the winning players.
    /// \throw runtime_error If the textures and src rects aren't the same length.
    /// \throw runtime_error If there are more winners provided than the function can handle.
    void ShowScore(int score, int highscore, std::vector<Texture *> texture_bird, std::vector<Rect *> frame_rect);

    void HideNewHighScoreButton();

    void ShowNewHighScoreButton();

    void Update(double delta_time);

    void Render(Renderer *renderer);

private:
    int index;
    int index_start;
    unsigned int max_players;
    bool showTitle = true;
    double timer_can_use_menu = 0;
    Texture *title;
    Texture *credits;
    std::vector<Texture *> bird;
    Texture *hand;
    std::vector<Texture *> start_buttons;
    Rect button_new_highscore_rect;
    bool is_new_high_score = false;
    Texture *button_new_highscore;
    Texture *arrow_left;
    Texture *arrow_right;
    Texture *winner_background;
    Texture *numbers;
    Rect title_rect;
    Rect credits_rect;
    std::vector<Rect> bird_rect;
    std::vector<Rect *> bird_src_rect;
    Rect hand_rect;
    std::vector<Rect> start_button_rects;
    Rect arrow_left_rect;
    Rect arrow_right_rect;
    Rect winner_background_rect;
    std::vector<Rect> numbers_frames;
    std::vector<std::pair<Rect, Rect>> score_rects;
    std::vector<std::pair<Rect, Rect>> best_score_rects;

    void SetHandPosition(int index);

    bool CanUseMenu();
};


#endif //LR_SUPERFLAPPYBIRDS_MENU_H
