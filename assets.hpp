#ifndef LR_SUPERFLAPPYBIRD_TEXTURES_H
#define LR_SUPERFLAPPYBIRD_TEXTURES_H

#include <vector>
#include "engine/audio.hpp"
#include "engine/image.hpp"
#include "engine/rect.hpp"
#include "engine/sound.hpp"
#include "engine/texture.hpp"

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
    Texture* bird5;
    Texture* bird6;
    Texture* bird7;
    Texture* bird8;
    Texture* numbers;
    Texture* score_background;
    Texture* start_1_player;
    Texture* start_2_player;
    Texture* start_3_player;
    Texture* start_4_player;
    Texture* start_5_player;
    Texture* start_6_player;
    Texture* start_7_player;
    Texture* start_8_player;
    Texture* start_single_player;
    Texture* arrow_left;
    Texture* arrow_right;
    Texture* hand;
    Texture* title;
    Texture* credits;
    Texture* winner_background;

    Sound* snd_hit;
    Sound* snd_score;

    std::vector<Rect> bird_frames;
    std::vector<Rect> numbers_frames;

    explicit Assets(const std::string &core_folder_path) {
        buildings = Image::LoadPNM(core_folder_path + "resources/buildings.pnm");
        ground = Image::LoadPNM(core_folder_path + "resources/ground.pnm");
        pipe_top = Image::LoadPNM(core_folder_path + "resources/pipe.pam");
        pipe_bottom = pipe_top->FlipHorizontally();
        bird = Image::LoadPNM(core_folder_path + "resources/bird.pam");
        bird2 = Image::LoadPNM(core_folder_path + "resources/bird2.pam");
        bird3 = Image::LoadPNM(core_folder_path + "resources/bird3.pam");
        bird4 = Image::LoadPNM(core_folder_path + "resources/bird4.pam");
        bird5 = Image::LoadPNM(core_folder_path + "resources/bird5.pam");
        bird6 = Image::LoadPNM(core_folder_path + "resources/bird6.pam");
        bird7 = Image::LoadPNM(core_folder_path + "resources/bird7.pam");
        bird8 = Image::LoadPNM(core_folder_path + "resources/bird8.pam");
        numbers = Image::LoadPNM(core_folder_path + "resources/numbers.pam");
        score_background = Image::LoadPNM(core_folder_path + "resources/score_background.pnm");
        start_1_player = Image::LoadPNM(core_folder_path + "resources/start_1_player.pnm");
        start_2_player = Image::LoadPNM(core_folder_path + "resources/start_2_player.pnm");
        start_3_player = Image::LoadPNM(core_folder_path + "resources/start_3_player.pnm");
        start_4_player = Image::LoadPNM(core_folder_path + "resources/start_4_player.pnm");
        start_5_player = Image::LoadPNM(core_folder_path + "resources/start_5_player.pnm");
        start_6_player = Image::LoadPNM(core_folder_path + "resources/start_6_player.pnm");
        start_7_player = Image::LoadPNM(core_folder_path + "resources/start_7_player.pnm");
        start_8_player = Image::LoadPNM(core_folder_path + "resources/start_8_player.pnm");
        start_single_player = Image::LoadPNM(core_folder_path + "resources/start_single_player.pnm");
        arrow_left = Image::LoadPNM(core_folder_path + "resources/arrow.pam");
        arrow_right = arrow_left->FlipVertical();
        hand = Image::LoadPNM(core_folder_path + "resources/hand.pam");
        title = Image::LoadPNM(core_folder_path + "resources/title.pam");
        credits = Image::LoadPNM(core_folder_path + "resources/credits.pnm");
        winner_background = Image::LoadPNM(core_folder_path + "resources/winner_background.pnm");

        bird_frames.emplace_back(0, 0, 17, 12);
        bird_frames.emplace_back(17, 0, 17, 12);
        bird_frames.emplace_back(34, 0, 17, 12);
        bird_frames.emplace_back(51, 0, 17, 12);

        numbers_frames.emplace_back(0,   0, 14, 20);
        numbers_frames.emplace_back(14,  0, 10, 20);
        numbers_frames.emplace_back(24,  0, 14, 20);
        numbers_frames.emplace_back(38,  0, 14, 20);
        numbers_frames.emplace_back(52,  0, 14, 20);
        numbers_frames.emplace_back(66,  0, 14, 20);
        numbers_frames.emplace_back(80,  0, 14, 20);
        numbers_frames.emplace_back(94,  0, 14, 20);
        numbers_frames.emplace_back(108, 0, 14, 20);
        numbers_frames.emplace_back(122, 0, 14, 20);

        snd_hit = Audio::LoadWav(core_folder_path + "resources/audio/hit.wav");
        snd_score = Audio::LoadWav(core_folder_path + "resources/audio/point.wav");
    }
};

#endif //LR_SUPERFLAPPYBIRD_TEXTURES_H
