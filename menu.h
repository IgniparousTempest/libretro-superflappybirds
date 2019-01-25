#ifndef LR_SUPERFLAPPYBIRDS_MENU_H
#define LR_SUPERFLAPPYBIRDS_MENU_H


#include <SDL_render.h>

class Menu {
public:
    Menu(SDL_Texture* texture_start_1_player, SDL_Texture* texture_start_2_player, SDL_Texture* texture_start_3_player, SDL_Texture* texture_start_4_player, SDL_Texture* texture_hand);
    void Left();
    void Right();
    ///
    /// \return The number of players for the next game.
    int Select();
    void Update();
    void Render(SDL_Renderer* renderer);

private:
    int index;
    SDL_Texture* hand;
    SDL_Texture* start_1_player;
    SDL_Texture* start_2_player;
    SDL_Texture* start_3_player;
    SDL_Texture* start_4_player;
    SDL_Rect hand_rect;
    SDL_Rect start_1_player_rect;
    SDL_Rect start_2_player_rect;
    SDL_Rect start_3_player_rect;
    SDL_Rect start_4_player_rect;

    void SetHandPosition(int index);
};


#endif //LR_SUPERFLAPPYBIRDS_MENU_H
