#ifndef SUPERFLAPPYBIRDS_LIBRETRO_HIGHSCORE_SUB_WINDOW_HPP
#define SUPERFLAPPYBIRDS_LIBRETRO_HIGHSCORE_SUB_WINDOW_HPP

#include "assets.hpp"
#include "engine/renderer.hpp"
#include "auxillary.hpp"
#include "engine/animation.hpp"
#include "bird.hpp"

class HighScoreSubWindow {
public:
    HighScoreSubWindow(int x, int y, Assets *assets, int player_number, Bird *bird, const std::vector<Rect> &bird_frames) : assets(assets), x(x), y(y), player_number(player_number), bird(bird), score(bird->score) {
        animation = new Animation(bird->animation->texture, bird_frames, Bird::FRAMES_PER_ANIMATION_FRAME);
        highscore_frame = assets->highscore_frame;
        player_name = bird->name;
        font = assets->font_highscore;
        cursor = assets->highscore_cursor;
        characters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ_\b\n";

        cursor_location = new Rect();
        SetPosition({x, y});
    }

    ~HighScoreSubWindow() {
        //TODO: I can't delete these, why?
//        delete position;
//        delete cursor_location;
//        delete animation;
//        delete bird_dest_rect;
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
        if (!finished)
            renderer->Render(cursor, cursor_location);
        renderer->Render(animation->texture, animation->CurrentFrame(), bird_dest_rect);
    }

    void SetPosition(Pos pos) {
        x = pos.x;
        y = pos.y;

        auto player_number_src_rects = assets->GetFontSrcRect("PLAYER " + std::to_string(player_number));
        player_number_rects = Auxillary::getFontRects(player_number_src_rects, x + 95, y + 32, 3, 3);
        auto score_src_rects = assets->GetFontSrcRect("SCORE: " + std::to_string(score));
        score_rects = Auxillary::getFontRects(score_src_rects, x + 29, y + 142, 2, 2);
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

        if (bird_dest_rect != nullptr)
            delete bird_dest_rect;
        if (position != nullptr)
            delete position;
        bird_dest_rect = new Rect(x + 226, y + 29, animation->CurrentFrame()->w * 2, animation->CurrentFrame()->h * 2);
        position = new Rect(x + 3, y + 3, highscore_frame->w, highscore_frame->h);
        UpdateCursor();
        UpdatePlayerName();
    }

    Pos GetPosition() {
        return Pos(x, y);
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
        else if (c == '\n') {
            finished = true;
            animation->Pause();
        }
        else if (player_name.size() < 8)
            player_name += c;
        UpdatePlayerName();
    }

    void Backspace() {
        if (!player_name.empty())
            player_name.pop_back();
        UpdatePlayerName();
    }

    bool IsFinished() {
        return finished;
    }

    std::string Name() {
        return player_name;
    }

    int Score() {
        return score;
    }

    Bird *GetBird() {
        return bird;
    }

private:
    bool finished = false;
    int x;
    int y;
    Assets *assets;
    int player_number;
    Bird *bird;
    Rect *position = nullptr;
    Texture *highscore_frame;
    Rect* bird_dest_rect = nullptr;
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
    int score;

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
