#ifndef LR_SUPERFLAPPYBIRD_TEXTURES_H
#define LR_SUPERFLAPPYBIRD_TEXTURES_H

#include <vector>
#include "engine/texture.hpp"
#include "engine/rect.hpp"
#include "engine/image.hpp"

class Assets {
public:
    Texture* buildings;
    Texture* ground;
    Texture* pipe_top;
    Texture* pipe_bottom;
    Texture* bird;
    Texture* bird2;
    Texture* bird3;
    Texture* bird4;
    Texture* numbers;
    Texture* score_background;
    Texture* start_1_player;
    Texture* start_2_player;
    Texture* start_3_player;
    Texture* start_4_player;
    Texture* hand;
    Texture* title;
    Texture* credits;
    Texture* winner_background;

    std::vector<Rect> bird_frames;
    std::vector<Rect> numbers_frames;

    Assets(std::string core_folder_path) {
        buildings = Image::LoadPNM(core_folder_path + "resources/buildings.pnm");
        ground = Image::LoadPNM(core_folder_path + "resources/ground.pnm");
        pipe_top = Image::LoadPNM(core_folder_path + "resources/pipe_top.pam");
        pipe_bottom = Image::LoadPNM(core_folder_path + "resources/pipe_bottom.pam");
        bird = Image::LoadPNM(core_folder_path + "resources/bird.pam");
        bird2 = Image::LoadPNM(core_folder_path + "resources/bird2.pam");
        bird3 = Image::LoadPNM(core_folder_path + "resources/bird3.pam");
        bird4 = Image::LoadPNM(core_folder_path + "resources/bird4.pam");
        numbers = Image::LoadPNM(core_folder_path + "resources/numbers.pam");
        score_background = Image::LoadPNM(core_folder_path + "resources/score_background.pnm");
        start_1_player = Image::LoadPNM(core_folder_path + "resources/start_1_player.pnm");
        start_2_player = Image::LoadPNM(core_folder_path + "resources/start_2_player.pnm");
        start_3_player = Image::LoadPNM(core_folder_path + "resources/start_3_player.pnm");
        start_4_player = Image::LoadPNM(core_folder_path + "resources/start_4_player.pnm");
        hand = Image::LoadPNM(core_folder_path + "resources/hand.pam");
        title = Image::LoadPNM(core_folder_path + "resources/title.pam");
        credits = Image::LoadPNM(core_folder_path + "resources/credits.pnm");
        winner_background = Image::LoadPNM(core_folder_path + "resources/winner_background.pnm");

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
