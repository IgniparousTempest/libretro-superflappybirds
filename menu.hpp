#ifndef LR_SUPERFLAPPYBIRDS_MENU_H
#define LR_SUPERFLAPPYBIRDS_MENU_H


#include <vector>
#include "engine/texture.hpp"
#include "engine/rect.hpp"
#include "engine/renderer.hpp"

class Menu {
public:
    Menu(Texture* texture_title, Texture* texture_credits, Texture* texture_start_1_player, Texture* texture_start_2_player, Texture* texture_start_3_player, Texture* texture_start_4_player, Texture *texture_start_5_player, Texture *texture_start_6_player, Texture *texture_start_7_player, Texture *texture_start_8_player, Texture *texture_arrow_left, Texture *texture_arrow_right, Texture* texture_hand, Texture* texture_winner_background, Texture* texture_numbers, std::vector<Rect> numbers_frames, unsigned int max_players);
    void Left();
    void Right();
    /// \return The number of players for the next game.
    int Select();
    void ShowScore(int score, int highscore, std::vector<Texture*> texture_bird, std::vector<Rect*> frame_rect);
    void Render(Renderer* renderer);

private:
    int index;
    int index_start;
    unsigned int max_players;
    bool showTitle = true;
    Texture* title;
    Texture* credits;
    std::vector<Texture*> bird;
    Texture* hand;
    std::vector<Texture*> start_buttons;
    Texture* arrow_left;
    Texture* arrow_right;
    Texture* winner_background;
    Texture* numbers;
    Rect title_rect;
    Rect credits_rect;
    std::vector<Rect> bird_rect;
    std::vector<Rect*> bird_src_rect;
    Rect hand_rect;
    std::vector<Rect> start_button_rects;
    Rect arrow_left_rect;
    Rect arrow_right_rect;
    Rect winner_background_rect;
    std::vector<Rect> numbers_frames;
    std::vector<std::pair<Rect, Rect>> score_rects;
    std::vector<std::pair<Rect, Rect>> best_score_rects;

    void SetHandPosition(int index);
};


#endif //LR_SUPERFLAPPYBIRDS_MENU_H
