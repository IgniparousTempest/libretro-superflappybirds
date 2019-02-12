#ifndef SUPERFLAPPYBIRDS_LIBRETRO_CONTEXT_HIGHSCORE_INPUT_HPP
#define SUPERFLAPPYBIRDS_LIBRETRO_CONTEXT_HIGHSCORE_INPUT_HPP


#include "context.hpp"
#include "../assets.hpp"
#include "../save_data.hpp"

class ContextHighScoreInput : public Context {
public:
    ContextHighScoreInput(Game *game, Assets *assets, SaveData *saveData, std::vector<int> scores,
            std::vector<Texture*> player_textures, std::vector<int> player_numbers);

    void Update(double delta_time, std::vector<Input> controller_inputs) override;

    void Render(Renderer *renderer) override;

private:
    Assets *assets;
    SaveData *saveData;
    std::vector<HighScoreSubWindow> highScoreWindows;
    std::vector<int> player_indexes;
};


#endif //SUPERFLAPPYBIRDS_LIBRETRO_CONTEXT_HIGHSCORE_INPUT_HPP
