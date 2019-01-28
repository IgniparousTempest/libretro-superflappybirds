#include <utility>
#include "menu.hpp"
#include "auxillary.hpp"

Menu::Menu(Texture* texture_title, Texture* texture_credits, Texture *texture_start_1_player,
           Texture *texture_start_2_player, Texture *texture_start_3_player,
           Texture *texture_start_4_player, Texture *texture_hand, Texture* texture_winner_background,
           Texture* texture_numbers, std::vector<Rect> numbers_frames) {
    index = 0;

    title = texture_title;
    credits = texture_credits;
    hand = texture_hand;
    start_1_player = texture_start_1_player;
    start_2_player = texture_start_2_player;
    start_3_player = texture_start_3_player;
    start_4_player = texture_start_4_player;
    winner_background = texture_winner_background;
    numbers = texture_numbers;
    this->numbers_frames = std::move(numbers_frames);

    title_rect = {154, 63, title->w, title->h};
    credits_rect = {266, 324, credits->w, credits->h};
    hand_rect = {0, 198, hand->w, hand->h};
    winner_background_rect = {272, 115, winner_background->w, winner_background->h};

    start_1_player_rect = {105, 230, start_1_player->w, start_1_player->h};
    start_2_player_rect = {217, 230, start_2_player->w, start_2_player->h};
    start_3_player_rect = {329, 230, start_3_player->w, start_3_player->h};
    start_4_player_rect = {441, 230, start_4_player->w, start_4_player->h};

    SetHandPosition(0);
}

void Menu::Left() {
    if (--index < 0)
        index = 0;
    SetHandPosition(index);
}

void Menu::Right() {
    if (++index >= 4)
        index = 3;
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
        bird_rect.push_back({bird_x, bird_y, bird_src_rect[0]->w * 2, bird_src_rect[0]->h * 2});
    else
        bird_rect.push_back({bird_x, bird_y, bird_src_rect[0]->w, bird_src_rect[0]->h});
    if (bird_src_rect.size() >= 2)
        bird_rect.push_back({bird_x + bird_src_rect[0]->w, bird_y, bird_src_rect[1]->w, bird_src_rect[1]->h});
    if (bird_src_rect.size() >= 3)
        bird_rect.push_back({bird_x, bird_y + bird_src_rect[0]->h, bird_src_rect[2]->w, bird_src_rect[2]->h});
    if (bird_src_rect.size() >= 4)
        bird_rect.push_back({bird_x + bird_src_rect[2]->w, bird_y + bird_src_rect[1]->h, bird_src_rect[3]->w, bird_src_rect[3]->h});
}

void Menu::Update() {

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
    renderer->Render(start_1_player, &start_1_player_rect);
    renderer->Render(start_2_player, &start_2_player_rect);
    renderer->Render(start_3_player, &start_3_player_rect);
    renderer->Render(start_4_player, &start_4_player_rect);
}

void Menu::SetHandPosition(int index) {
    Rect* button_rect;
    switch (index) {
        case 0:
            button_rect = &start_1_player_rect;
            break;
        case 1:
            button_rect = &start_2_player_rect;
            break;
        case 2:
            button_rect = &start_3_player_rect;
            break;
        default:
            button_rect = &start_4_player_rect;
            break;
    }
    hand_rect.x = button_rect->x + button_rect->w / 2 - hand_rect.w / 2;
}
