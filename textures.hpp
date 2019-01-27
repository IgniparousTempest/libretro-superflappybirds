#ifndef LR_SUPERFLAPPYBIRD_TEXTURES_H
#define LR_SUPERFLAPPYBIRD_TEXTURES_H

#include <SDL_render.h>
#include <vector>

class Textures {
public:
    SDL_Texture* sky;
    SDL_Texture* buildings;
    SDL_Texture* ground;
    SDL_Texture* pipe_top;
    SDL_Texture* pipe_bottom;
    SDL_Texture* bird;
    SDL_Texture* bird2;
    SDL_Texture* bird3;
    SDL_Texture* bird4;
    SDL_Texture* numbers;
    SDL_Texture* score_background;
    SDL_Texture* start_1_player;
    SDL_Texture* start_2_player;
    SDL_Texture* start_3_player;
    SDL_Texture* start_4_player;
    SDL_Texture* hand;
    SDL_Texture* title;
    SDL_Texture* credits;
    SDL_Texture* winner_background;

    int sky_w;
    int buildings_w;
    int ground_w;
    int pipe_top_w;
    int pipe_bottom_w;
    int score_background_w;

    int sky_h;
    int buildings_h;
    int ground_h;
    int pipe_top_h;
    int pipe_bottom_h;
    int score_background_h;

    std::vector<SDL_Rect> bird_frames;
    std::vector<SDL_Rect> numbers_frames;

    explicit Textures(SDL_Renderer *renderer) {
        SDL_Surface *sky = SDL_LoadBMP("resources/sky.bmp");
        SDL_Surface *buildings = SDL_LoadBMP("resources/buildings.bmp");
        SDL_Surface *ground = SDL_LoadBMP("resources/ground.bmp");
        SDL_Surface *pipe_top = SDL_LoadBMP("resources/pipe_top.bmp");
        SDL_Surface *pipe_bottom = SDL_LoadBMP("resources/pipe_bottom.bmp");
        SDL_Surface *bird = SDL_LoadBMP("resources/bird.bmp");
        SDL_Surface *bird2 = SDL_LoadBMP("resources/bird2.bmp");
        SDL_Surface *bird3 = SDL_LoadBMP("resources/bird3.bmp");
        SDL_Surface *bird4 = SDL_LoadBMP("resources/bird4.bmp");
        SDL_Surface *numbers = SDL_LoadBMP("resources/numbers.bmp");
        SDL_Surface *score_background = SDL_LoadBMP("resources/score_background.bmp");
        SDL_Surface *start_1_player = SDL_LoadBMP("resources/start_1_player.bmp");
        SDL_Surface *start_2_player = SDL_LoadBMP("resources/start_2_player.bmp");
        SDL_Surface *start_3_player = SDL_LoadBMP("resources/start_3_player.bmp");
        SDL_Surface *start_4_player = SDL_LoadBMP("resources/start_4_player.bmp");
        SDL_Surface *hand = SDL_LoadBMP("resources/hand.bmp");
        SDL_Surface *title = SDL_LoadBMP("resources/title.bmp");
        SDL_Surface *credits = SDL_LoadBMP("resources/credits.bmp");
        SDL_Surface *winner_background = SDL_LoadBMP("resources/winner_background.bmp");

        this->sky = SDL_CreateTextureFromSurface(renderer, sky);
        this->buildings = SDL_CreateTextureFromSurface(renderer, buildings);
        this->ground = SDL_CreateTextureFromSurface(renderer, ground);
        this->pipe_top = SDL_CreateTextureFromSurface(renderer, pipe_top);
        this->pipe_bottom = SDL_CreateTextureFromSurface(renderer, pipe_bottom);
        this->bird = SDL_CreateTextureFromSurface(renderer, bird);
        this->bird2 = SDL_CreateTextureFromSurface(renderer, bird2);
        this->bird3 = SDL_CreateTextureFromSurface(renderer, bird3);
        this->bird4 = SDL_CreateTextureFromSurface(renderer, bird4);
        this->numbers = SDL_CreateTextureFromSurface(renderer, numbers);
        this->score_background = SDL_CreateTextureFromSurface(renderer, score_background);
        this->start_1_player = SDL_CreateTextureFromSurface(renderer, start_1_player);
        this->start_2_player = SDL_CreateTextureFromSurface(renderer, start_2_player);
        this->start_3_player = SDL_CreateTextureFromSurface(renderer, start_3_player);
        this->start_4_player = SDL_CreateTextureFromSurface(renderer, start_4_player);
        this->hand = SDL_CreateTextureFromSurface(renderer, hand);
        this->title = SDL_CreateTextureFromSurface(renderer, title);
        this->credits = SDL_CreateTextureFromSurface(renderer, credits);
        this->winner_background = SDL_CreateTextureFromSurface(renderer, winner_background);

        sky_w = sky->w;
        buildings_w = buildings->w;
        ground_w = ground->w;
        pipe_top_w = pipe_top->w;
        pipe_bottom_w = pipe_bottom->w;
        score_background_w = score_background->w;

        sky_h = sky->h;
        buildings_h = buildings->h;
        ground_h = ground->h;
        pipe_top_h = pipe_top->h;
        pipe_bottom_h = pipe_bottom->h;
        score_background_h = score_background->h;

        SDL_FreeSurface(sky);
        SDL_FreeSurface(buildings);
        SDL_FreeSurface(ground);
        SDL_FreeSurface(pipe_top);
        SDL_FreeSurface(pipe_bottom);
        SDL_FreeSurface(bird);
        SDL_FreeSurface(bird2);
        SDL_FreeSurface(bird3);
        SDL_FreeSurface(bird4);
        SDL_FreeSurface(numbers);
        SDL_FreeSurface(score_background);
        SDL_FreeSurface(start_1_player);
        SDL_FreeSurface(start_2_player);
        SDL_FreeSurface(start_3_player);
        SDL_FreeSurface(start_4_player);
        SDL_FreeSurface(hand);
        SDL_FreeSurface(title);
        SDL_FreeSurface(credits);
        SDL_FreeSurface(winner_background);

        bird_frames.push_back({0, 0, 17, 12});
        bird_frames.push_back({17, 0, 17, 12});
        bird_frames.push_back({34, 0, 17, 12});
        bird_frames.push_back({51, 0, 17, 12});

        numbers_frames.push_back({0,   0, 14, 20});
        numbers_frames.push_back({14,  0, 10, 20});
        numbers_frames.push_back({24,  0, 14, 20});
        numbers_frames.push_back({38,  0, 14, 20});
        numbers_frames.push_back({52,  0, 14, 20});
        numbers_frames.push_back({66,  0, 14, 20});
        numbers_frames.push_back({80,  0, 14, 20});
        numbers_frames.push_back({94,  0, 14, 20});
        numbers_frames.push_back({108, 0, 14, 20});
        numbers_frames.push_back({122, 0, 14, 20});
    }
};

#endif //LR_SUPERFLAPPYBIRD_TEXTURES_H
