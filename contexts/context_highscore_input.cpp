#include "context_highscore_input.hpp"

ContextHighScoreInput::ContextHighScoreInput(Game *game, Assets *assets, SaveData *saveData, std::vector<int> scores,
        std::vector<Texture*> player_textures, std::vector<int> player_numbers) : Context(game) {
    assert(scores.size() == player_numbers.size() && scores.size() == player_textures.size());

    for (int player_number : player_numbers)
        player_indexes.push_back(player_number - 1);

    size_t high_score_count = scores.size();
    if (high_score_count > 4) {
        std::cout << "Trimmed number of high scorers from " << high_score_count << " to 4." << std::endl;
        high_score_count = 4;
    }

    for (int i = 0; i < high_score_count; ++i)
        highScoreWindows.emplace_back(0, 0, assets, player_numbers[i], player_textures[i], assets->bird_frames, scores[i]);
}

void ContextHighScoreInput::Update(double delta_time, std::vector<Input> controller_inputs) {
    Input* input;
    for (int i = 0; i < player_indexes.size(); ++i) {
        input = &controller_inputs[player_indexes[i]];
        if (input->up_pressed)
            highScoreWindows[i].Up();
        if (input->down_pressed)
            highScoreWindows[i].Down();
        if (input->left_pressed)
            highScoreWindows[i].Left();
        if (input->right_pressed)
            highScoreWindows[i].Right();
        if (input->flap_pressed) {
            highScoreWindows[i].Select();
        }
    }
}

void ContextHighScoreInput::Render(Renderer *renderer) {
    for (auto &highScoreWindow : highScoreWindows)
        highScoreWindow.Render(renderer);
}


