#ifndef LR_SUPERFLAPPYBIRDS_MENU_H
#define LR_SUPERFLAPPYBIRDS_MENU_H


#include <vector>
#include "engine/texture.hpp"
#include "engine/rect.hpp"
#include "engine/renderer.hpp"

class Menu {
public:
    Menu(Texture* texture_title, Texture* texture_credits, Texture* texture_start_1_player, Texture* texture_start_2_player, Texture* texture_start_3_player, Texture* texture_start_4_player, Texture* texture_hand, Texture* texture_winner_background, Texture* texture_numbers, std::vector<Rect> numbers_frames);
    void Left();
    void Right();
    /// \return The number of players for the next game.
    int Select();
    void ShowScore(int score, int highscore, std::vector<Texture*> texture_bird, std::vector<Rect*> frame_rect);
    void Update();
    void Render(Renderer* renderer);

private:
    int index;
    bool showTitle = true;
    Texture* title;
    Texture* credits;
    std::vector<Texture*> bird;
    Texture* hand;
    Texture* start_1_player;
    Texture* start_2_player;
    Texture* start_3_player;
    Texture* start_4_player;
    Texture* winner_background;
    Texture* numbers;
    Rect title_rect;
    Rect credits_rect;
    std::vector<Rect> bird_rect;
    std::vector<Rect*> bird_src_rect;
    Rect hand_rect;
    Rect start_1_player_rect;
    Rect start_2_player_rect;
    Rect start_3_player_rect;
    Rect start_4_player_rect;
    Rect winner_background_rect;
    std::vector<Rect> numbers_frames;
    std::vector<std::pair<Rect, Rect>> score_rects;
    std::vector<std::pair<Rect, Rect>> best_score_rects;

    void SetHandPosition(int index);
};


#endif //LR_SUPERFLAPPYBIRDS_MENU_H
