#ifndef SUPERFLAPPYBIRDS_LIBRETRO_CONTEXT_LOAD_OLD_SAVE_FILE_HPP
#define SUPERFLAPPYBIRDS_LIBRETRO_CONTEXT_LOAD_OLD_SAVE_FILE_HPP


#include "context.hpp"
#include "../highscore_sub_window.hpp"

class ContextLoadOldSaveFile : public Context {
public:
    /// This context allows a user to enter their name for a high score for an old format save file.
    ContextLoadOldSaveFile(GameManager *game, Assets *assets, SaveData *save_data, std::string file_path);

    void Update(double delta_time, std::vector<Input> controller_inputs) override;

    void Render(Renderer *renderer) override;

private:
    Assets *assets;
    SaveData *save_data;
    std::vector<std::pair<Rect, Rect>> title_rects;
    Texture *font;
    HighScoreSubWindow *high_score_sub_window;
    bool finished = false;
    int score;
    std::string save_file_path;

    void EndContext();
};


#endif //SUPERFLAPPYBIRDS_LIBRETRO_CONTEXT_LOAD_OLD_SAVE_FILE_HPP
