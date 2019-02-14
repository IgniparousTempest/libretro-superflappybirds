#include "context_highscore_input.hpp"

ContextHighScoreInput::ContextHighScoreInput(GameManager *game, Assets *assets, SaveData *saveData,
        std::vector<Bird*> player_birds, std::vector<int> player_numbers) : Context(game), saveData(saveData) {
    assert(player_birds.size() == player_numbers.size());

    for (int player_number : player_numbers)
        player_indexes.push_back(player_number - 1);

    size_t high_score_count = player_birds.size();
    if (high_score_count > 4) {
        std::cout << "Trimmed number of high scorers from " << high_score_count << " to 4." << std::endl;
        high_score_count = 4;
    }

    int x, y;
    for (int i = 0; i < high_score_count; ++i) {
        x = (gameManager->ScreenWidth() / 2) * (i % 2);
        y = (gameManager->ScreenHeight() / 2) * (i / 2);
        highScoreWindows.emplace_back(x, y, assets, player_numbers[i], player_birds[i], assets->bird_frames);
    }
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
        if (input->flap_pressed)
            highScoreWindows[i].Select();
    }

    bool all_finished = true;
    for (auto &highScoreWindow : highScoreWindows)
        if (!highScoreWindow.IsFinished()) {
            all_finished = false;
            break;
        }
    if (all_finished) {
        for (auto &highScoreWindow : highScoreWindows) {
            if (highScoreWindow.Name() != "") {
                std::cout << "Adding a new high score to table, name=\"" << highScoreWindow.Name() << "\", score=" << highScoreWindow.Score() << std::endl;
                highScoreWindow.GetBird()->name = highScoreWindow.Name();
                saveData->AddNewScore(highScoreWindow.Name(), highScoreWindow.Score());
            } else
                std::cout << "The player entered a blank name, they don't deserve to be added to the high score table." << std::endl;
        }
        saveData->Serialize();
        gameManager->EndCurrentContext();
        return;
    }
}

void ContextHighScoreInput::Render(Renderer *renderer) {
    // Draw Sky
    renderer->Clear(Renderer::rgb(115, 183, 196));

    for (auto &highScoreWindow : highScoreWindows)
        highScoreWindow.Render(renderer);
}


