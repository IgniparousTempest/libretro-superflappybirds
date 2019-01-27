#ifndef LR_SUPERFLAPPYBIRDS_MENU_H
#define LR_SUPERFLAPPYBIRDS_MENU_H


#include <SDL_render.h>
#include <vector>

class Menu {
public:
    Menu(SDL_Texture* texture_title, SDL_Texture* texture_credits, SDL_Texture* texture_start_1_player, SDL_Texture* texture_start_2_player, SDL_Texture* texture_start_3_player, SDL_Texture* texture_start_4_player, SDL_Texture* texture_hand, SDL_Texture* texture_winner_background);
    void Left();
    void Right();
    /// \return The number of players for the next game.
    int Select();
    void ShowScore(std::vector<SDL_Texture*> texture_bird, std::vector<SDL_Rect*> frame_rect);
    void Update();
    void Render(SDL_Renderer* renderer);

private:
    int index;
    bool showTitle = true;
    SDL_Texture* title;
    SDL_Texture* credits;
    std::vector<SDL_Texture*> bird;
    SDL_Texture* hand;
    SDL_Texture* start_1_player;
    SDL_Texture* start_2_player;
    SDL_Texture* start_3_player;
    SDL_Texture* start_4_player;
    SDL_Texture* winner_background;
    SDL_Rect title_rect;
    SDL_Rect credits_rect;
    std::vector<SDL_Rect> bird_rect;
    std::vector<SDL_Rect*> bird_src_rect;
    SDL_Rect hand_rect;
    SDL_Rect start_1_player_rect;
    SDL_Rect start_2_player_rect;
    SDL_Rect start_3_player_rect;
    SDL_Rect start_4_player_rect;
    SDL_Rect winner_background_rect;

    void SetHandPosition(int index);
};


#endif //LR_SUPERFLAPPYBIRDS_MENU_H
