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
    Texture* score_wins_background;
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
    Texture* font_highscore;
    Texture* highscore_frame;
    Texture* highscore_cursor;

    Sound* snd_hit;
    Sound* snd_score;

    std::vector<Rect> bird_frames;
    std::vector<Rect> numbers_frames;
    std::vector<Rect> font_highscore_frames;

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
        score_wins_background = Image::LoadPNM(core_folder_path + "resources/score_background_multiplayer.pnm");
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
        font_highscore = Image::LoadPNM(core_folder_path + "resources/font_highscore.pam");
        highscore_frame = Image::LoadPNM(core_folder_path + "resources/highscore_frame.pam");
        highscore_cursor = Image::LoadPNM(core_folder_path + "resources/highscore_cursor.pam");

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

    Rect GetFontSrcRect(char letter) {
        switch (letter) {
            case 'A': return {0, 0, 4, 6};
            case 'B': return {5, 0, 4, 6};
            case 'C': return {10, 0, 3, 6};
            case 'D': return {14, 0, 4, 6};
            case 'E': return {19, 0, 3, 6};
            case 'F': return {23, 0, 3, 6};
            case 'G': return {27, 0, 4, 6};
            case 'H': return {32, 0, 4, 6};
            case 'I': return {37, 0, 3, 6};
            case 'J': return {41, 0, 3, 6};
            case 'K': return {45, 0, 4, 6};
            case 'L': return {50, 0, 3, 6};
            case 'M': return {54, 0, 5, 6};
            case 'N': return {60, 0, 4, 6};
            case 'O': return {65, 0, 4, 6};
            case 'P': return {70, 0, 4, 6};
            case 'Q': return {75, 0, 4, 6};
            case 'R': return {80, 0, 4, 6};
            case 'S': return {85, 0, 4, 6};
            case 'T': return {90, 0, 3, 6};
            case 'U': return {94, 0, 4, 6};
            case 'V': return {99, 0, 3, 6};
            case 'W': return {103, 0, 5, 6};
            case 'X': return {109, 0, 3, 6};
            case 'Y': return {113, 0, 3, 6};
            case 'Z': return {117, 0, 4, 6};
            case '0': return {122, 0, 4, 6};
            case '1': return {127, 0, 3, 6};
            case '2': return {131, 0, 4, 6};
            case '3': return {136, 0, 4, 6};
            case '4': return {141, 0, 4, 6};
            case '5': return {146, 0, 4, 6};
            case '6': return {151, 0, 4, 6};
            case '7': return {156, 0, 4, 6};
            case '8': return {161, 0, 4, 6};
            case '9': return {166, 0, 4, 6};
            case ' ': return {171, 0, 4, 6};
            case '_': return {176, 0, 4, 6};
            case ':': return {181, 0, 1, 6};
            case '.': return {183, 0, 1, 6};
            case '\b': return {185, 0, 11, 12};
            case '\n': return {197, 0, 12, 12};
            default:
                throw std::runtime_error("Unknown letter '" + std::to_string(letter) + "'");
        }
    }

    std::vector<Rect> GetFontSrcRect(const std::string &text) {
        std::vector<Rect> rects;
        for (char i : text) {
            rects.push_back(GetFontSrcRect(i));
        }
        return rects;
    }
};

#endif //LR_SUPERFLAPPYBIRD_TEXTURES_H
