#ifndef SUPERFLAPPYBIRDS_LIBRETRO_CONTEXT_HIGH_SCORE_VIEW_HPP
#define SUPERFLAPPYBIRDS_LIBRETRO_CONTEXT_HIGH_SCORE_VIEW_HPP


#include "context.hpp"

class ContextHighScoreView : public Context {
public:
    /// This context shows the high scores that have been achieved in this game.
    ContextHighScoreView(GameManager *game, Assets *assets, SaveData *save_data);

    void Update(double delta_time, std::vector<Input> controller_inputs) override;

    void Render(Renderer *renderer) override;

private:
    Assets *assets;
    SaveData *save_data;
    std::vector<std::pair<Rect, Rect>> title_rects;
    std::vector<std::pair<Rect, Rect>> quit_message;
    std::vector<std::vector<std::pair<Rect, Rect>>> scores_rank_rects;
    std::vector<std::vector<std::pair<Rect, Rect>>> scores_score_rects;
    std::vector<std::vector<std::pair<Rect, Rect>>> scores_names_rects;
    Texture *font;
    double alive_timer = 15;
};


#endif //SUPERFLAPPYBIRDS_LIBRETRO_CONTEXT_HIGH_SCORE_VIEW_HPP
