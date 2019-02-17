#include "context_high_score_view.hpp"
#include "../auxillary.hpp"

ContextHighScoreView::ContextHighScoreView(GameManager *game, Assets *assets, SaveData *save_data) : Context(game, "View High Scores"),
save_data(save_data), assets(assets) {
    font = assets->font_highscore;

    auto title_src_rects = assets->GetFontSrcRect("THE FLAPPIEST BIRDS OF ALL TIME...");
    title_rects = Auxillary::getFontRects(title_src_rects, 0, 20, 3, 4);
    int width = title_rects.back().second.x + title_rects.back().second.w;
    for (auto &high_score_rect : title_rects)
        high_score_rect.second.x += (gameManager->ScreenWidth() - width) / 2;

    auto quit_message_src_rects = assets->GetFontSrcRect("PRESS \"FLAP\" BUTTON TO QUIT.");
    quit_message = Auxillary::getFontRects(quit_message_src_rects, 0, 0, 2, 2);
    width = quit_message.back().second.x + quit_message.back().second.w;
    int height = quit_message.back().second.h;
    for (auto &high_score_rect : quit_message) {
        high_score_rect.second.x += (gameManager->ScreenWidth() - width) / 2;
        high_score_rect.second.y += gameManager->ScreenHeight() - height - 2;
    }

    int rank_x = 160, score_x = 270, name_x = 380;
    auto src_rects = assets->GetFontSrcRect("RANK");
    scores_rank_rects.push_back(Auxillary::getFontRects(src_rects, rank_x, 60, 3, 3));
    src_rects = assets->GetFontSrcRect("SCORE");
    scores_score_rects.push_back(Auxillary::getFontRects(src_rects, score_x, 60, 3, 3));
    src_rects = assets->GetFontSrcRect("NAME");
    scores_names_rects.push_back(Auxillary::getFontRects(src_rects, name_x, 60, 3, 3));
    for (int i = 0; i < save_data->top_scores.size(); ++i) {
        src_rects = assets->GetFontSrcRect("NO. " + std::to_string(i + 1));
        scores_rank_rects.push_back(Auxillary::getFontRects(src_rects, rank_x, 60 + (i + 1) * 25, 3, 3));

        src_rects = assets->GetFontSrcRect(std::to_string(save_data->top_scores[i].score));
        scores_score_rects.push_back(Auxillary::getFontRects(src_rects, score_x, 60 + (i + 1) * 25, 3, 3));

        src_rects = assets->GetFontSrcRect(save_data->top_scores[i].name);
        scores_names_rects.push_back(Auxillary::getFontRects(src_rects, name_x, 60 + (i + 1) * 25, 3, 3));
    }
}

void ContextHighScoreView::Update(double delta_time, std::vector<Input> controller_inputs) {
    alive_timer -= delta_time;
    for (auto &input : controller_inputs)
        if (input.flap_pressed)
            alive_timer = -1;
    if (alive_timer < 0) {
        gameManager->EndCurrentContext();
        return;
    }
}

void ContextHighScoreView::Render(Renderer *renderer) {
    // Draw Sky
    renderer->Clear(Renderer::rgb(115, 183, 196));

    for (auto &rect : title_rects)
        renderer->Render(font, &rect.first, &rect.second);

    for (auto &score : scores_rank_rects)
        for (auto &rect : score)
            renderer->Render(font, &rect.first, &rect.second);
    for (auto &score : scores_score_rects)
        for (auto &rect : score)
            renderer->Render(font, &rect.first, &rect.second);
    for (auto &score : scores_names_rects)
        for (auto &rect : score)
            renderer->Render(font, &rect.first, &rect.second);

    for (auto &rect : quit_message)
        renderer->Render(font, &rect.first, &rect.second);
}
