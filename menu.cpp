#include <utility>
#include "menu.hpp"
#include "auxillary.hpp"

Menu::Menu(Texture* texture_title, Texture* texture_credits, Texture *texture_start_1_player,
           Texture *texture_start_2_player, Texture *texture_start_3_player, Texture *texture_start_4_player,
           Texture *texture_start_5_player, Texture *texture_start_6_player, Texture *texture_start_7_player,
           Texture *texture_start_8_player, Texture *texture_arrow_left, Texture *texture_arrow_right,
           Texture *texture_hand, Texture* texture_winner_background, Texture* texture_numbers,
           std::vector<Rect> numbers_frames, unsigned int max_players) {
    index = 0;
    index_start = 0;
    this->max_players = max_players;

    title = texture_title;
    credits = texture_credits;
    hand = texture_hand;
    start_buttons = {texture_start_1_player, texture_start_2_player, texture_start_3_player, texture_start_4_player, texture_start_5_player, texture_start_6_player, texture_start_7_player, texture_start_8_player};
    arrow_left = texture_arrow_left;
    arrow_right = texture_arrow_right;
    winner_background = texture_winner_background;
    numbers = texture_numbers;
    this->numbers_frames = std::move(numbers_frames);

    title_rect = {154, 63, title->w, title->h};
    credits_rect = {266, 324, credits->w, credits->h};
    hand_rect = {0, 198, hand->w, hand->h};
    switch (max_players) {
        case 1:
            start_button_rects = {
                    {273, 230, start_buttons[0]->w, start_buttons[0]->h}
            };
            break;
        case 2:
            start_button_rects = {
                    {217, 230, start_buttons[0]->w, start_buttons[0]->h},
                    {329, 230, start_buttons[0]->w, start_buttons[0]->h}
            };
            break;
        case 3:
            start_button_rects = {
                    {161, 230, start_buttons[0]->w, start_buttons[0]->h},
                    {273, 230, start_buttons[0]->w, start_buttons[0]->h},
                    {385, 230, start_buttons[0]->w, start_buttons[0]->h}
            };
            break;
        default:
            start_button_rects = {
                    {105, 230, start_buttons[0]->w, start_buttons[0]->h},
                    {217, 230, start_buttons[0]->w, start_buttons[0]->h},
                    {329, 230, start_buttons[0]->w, start_buttons[0]->h},
                    {441, 230, start_buttons[0]->w, start_buttons[0]->h}
            };
            break;
    }
    arrow_left_rect = {73, 231, arrow_left->w, arrow_left->h};
    arrow_right_rect = {549, 231, arrow_right->w, arrow_right->h};
    winner_background_rect = {272, 115, winner_background->w, winner_background->h};

    SetHandPosition(0);
}

void Menu::Left() {
    if (--index < 0)
        index = 0;
    if (index < index_start)
        index_start = index;
    SetHandPosition(index);
}

void Menu::Right() {
    if (++index >= max_players)
        index = max_players - 1;
    if (index >= index_start + 4)
        index_start = index - 3;
    SetHandPosition(index);
}

int Menu::Select() {
    return index + 1;
}

void Menu::ShowScore(int score, int highscore, std::vector<Texture*> texture_bird, std::vector<Rect*> frame_rect) {
    showTitle = false;

    // Score
    score_rects = Auxillary::getNumberRects(score, &numbers_frames, 0, winner_background_rect.y + 17, 1);
    int score_x = winner_background_rect.x + 91 - (score_rects.back().second.x + score_rects.back().second.w) / 2;
    for (auto &rect_pair : score_rects)
        rect_pair.second.x += score_x;

    // Best
    best_score_rects = Auxillary::getNumberRects(highscore, &numbers_frames, 0, winner_background_rect.y + 47, 1, 0.5);
    score_x = winner_background_rect.x + 93 - (best_score_rects.back().second.x + best_score_rects.back().second.w) / 2;
    for (auto &rect_pair : best_score_rects)
        rect_pair.second.x += score_x;

    // Place players birds
    bird = std::move(texture_bird);
    bird_src_rect = std::move(frame_rect);
    bird_rect = {};
    int bird_x = winner_background_rect.x + 10;
    int bird_y = winner_background_rect.y + 23;
    if (bird_src_rect.size() == 1)
        bird_rect.emplace_back(bird_x, bird_y, bird_src_rect[0]->w * 2, bird_src_rect[0]->h * 2);
    else
        bird_rect.emplace_back(bird_x, bird_y, bird_src_rect[0]->w, bird_src_rect[0]->h);
    if (bird_src_rect.size() >= 2)
        bird_rect.emplace_back(bird_x + bird_src_rect[0]->w, bird_y, bird_src_rect[1]->w, bird_src_rect[1]->h);
    if (bird_src_rect.size() >= 3)
        bird_rect.emplace_back(bird_x, bird_y + bird_src_rect[0]->h, bird_src_rect[2]->w, bird_src_rect[2]->h);
    if (bird_src_rect.size() >= 4)
        bird_rect.emplace_back(bird_x + bird_src_rect[2]->w, bird_y + bird_src_rect[1]->h, bird_src_rect[3]->w, bird_src_rect[3]->h);
}

void Menu::Render(Renderer *renderer) {
    if (showTitle) {
        renderer->Render(title, &title_rect);
        renderer->Render(credits, &credits_rect);
    } else {
        renderer->Render(winner_background, &winner_background_rect);
        for (int i = 0; i < bird.size(); ++i)
            renderer->Render(bird[i], bird_src_rect[i], &bird_rect[i]);
        for (auto rect_pair : score_rects)
            renderer->Render(numbers, &rect_pair.first, &rect_pair.second);
        for (auto rect_pair : best_score_rects)
            renderer->Render(numbers, &rect_pair.first, &rect_pair.second);
    }
    renderer->Render(hand, &hand_rect);
    for (int i = 0; i < start_button_rects.size(); ++i)
        renderer->Render(start_buttons[index_start + i], &start_button_rects[i]);
    if (index_start != 0)
        renderer->Render(arrow_left, &arrow_left_rect);
    if (index_start != max_players - start_button_rects.size())
        renderer->Render(arrow_right, &arrow_right_rect);
}

void Menu::SetHandPosition(int index) {
    Rect* button_rect = &start_button_rects[index - index_start];
    hand_rect.x = button_rect->x + button_rect->w / 2 - hand_rect.w / 2;
}
