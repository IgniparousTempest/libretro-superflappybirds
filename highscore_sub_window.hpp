#ifndef SUPERFLAPPYBIRDS_LIBRETRO_HIGHSCORE_SUB_WINDOW_HPP
#define SUPERFLAPPYBIRDS_LIBRETRO_HIGHSCORE_SUB_WINDOW_HPP

#include "assets.hpp"
#include "engine/renderer.hpp"
#include "auxillary.hpp"

class HighScoreSubWindow {
public:
    HighScoreSubWindow(int x, int y, Assets *assets, int player_number, Texture *bird, std::vector<Rect> bird_frames, int score) : assets(assets), x(x), y(y) {
        animation = new Animation(bird, std::move(bird_frames), Bird::FRAMES_PER_ANIMATION_FRAME);
        highscore_frame = assets->highscore_frame;
        font = assets->font_highscore;
        cursor = assets->highscore_cursor;
        auto player_number_src_rects = assets->GetFontSrcRect("PLAYER " + std::to_string(player_number));
        player_number_rects = Auxillary::getFontRects(player_number_src_rects, x + 95, y + 32, 3, 3);
        auto score_src_rects = assets->GetFontSrcRect("SCORE: " + std::to_string(score));
        score_rects = Auxillary::getFontRects(score_src_rects, x + 29, y + 142, 2, 2);
        characters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ_\b\n";
        auto keyboard_src_rects = assets->GetFontSrcRect(characters);

        int i = 0;
        int scale;
        for (auto &src_rect : keyboard_src_rects) {
            if (characters[i] == '\b' || characters[i] == '\n')
                scale = 1;
            else
                scale = 2;
            Rect dest;
            dest.x = x + 31 + (i % 10) * 28;
            dest.y = y + 62 + (i / 10) * 25;
            dest.w = src_rect.w * scale;
            dest.h = src_rect.h * scale;
            keyboard_rects.emplace_back(src_rect, dest);
            i++;
        }

        bird_dest_rect = new Rect(x + 226, y + 29, animation->CurrentFrame()->w * 2, bird->h * 2);
        position = new Rect(x + 3, y + 3, highscore_frame->w, highscore_frame->h);
        cursor_location = new Rect();
        UpdateCursor();
        UpdatePlayerName();
    }

    ~HighScoreSubWindow() {
        delete position;
        delete cursor_location;
        delete animation;
        delete bird_dest_rect;
    }

    void Render(Renderer *renderer) {
        animation->Update();

        renderer->Render(highscore_frame, position);
        for (auto &rect : player_number_rects)
            renderer->Render(font, &rect.first, &rect.second);
        for (auto &rect : score_rects)
            renderer->Render(font, &rect.first, &rect.second);
        for (auto &rect : player_name_rects)
            renderer->Render(font, &rect.first, &rect.second);
        for (auto &rect : keyboard_rects)
            renderer->Render(font, &rect.first, &rect.second);
        renderer->Render(cursor, cursor_location);
        renderer->Render(animation->texture, animation->CurrentFrame(), bird_dest_rect);
    }

    void Up() {
        if (cursor_index >= 10)
            cursor_index = std::max(0, cursor_index - 10);
        UpdateCursor();
    }

    void Down() {
        if (cursor_index < (characters.size() / 10) * 10)
            cursor_index = std::min((int)(characters.size() - 1), cursor_index + 10);
        UpdateCursor();
    }

    void Left() {
        cursor_index = std::max(0, cursor_index - 1);
        UpdateCursor();
    }

    void Right() {
        cursor_index = std::min((int)(characters.size() - 1), cursor_index + 1);
        UpdateCursor();
    }

    void Select() {
        char c = characters[cursor_index];
        if (c == '_')
            c = ' ';

        if (c == '\b') {
            if (!player_name.empty())
                player_name.pop_back();
        }
        else if (c == '\n')
            c;
        else if (player_name.size() < 8)
            player_name += c;
        UpdatePlayerName();
    }

private:
    int x;
    int y;
    Assets *assets;
    Rect *position;
    Texture *highscore_frame;
    Rect* bird_dest_rect;
    Animation* animation;
    Texture *cursor;
    Rect *cursor_location;
    Texture *font;
    std::string characters;
    std::string player_name;
    std::vector<std::pair<Rect, Rect>> player_number_rects;
    std::vector<std::pair<Rect, Rect>> player_name_rects;
    std::vector<std::pair<Rect, Rect>> score_rects;
    std::vector<std::pair<Rect, Rect>> keyboard_rects;
    int cursor_index = 0;

    void UpdateCursor() {
        auto current_letter = &keyboard_rects[cursor_index].second;
        cursor_location->x = current_letter->x + current_letter->w / 2 - cursor->w / 2;
        cursor_location->y = current_letter->y - 8;
        cursor_location->w = cursor->w;
        cursor_location->h = cursor->h;
    }

    void UpdatePlayerName() {
        auto player_name_src_rects = assets->GetFontSrcRect("NAME: " + player_name);
        player_name_rects = Auxillary::getFontRects(player_name_src_rects, x + 140, y + 142, 2, 2);
    }
};

#endif //SUPERFLAPPYBIRDS_LIBRETRO_HIGHSCORE_SUB_WINDOW_HPP