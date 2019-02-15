#include "context_highscore_input.hpp"

ContextHighScoreInput::ContextHighScoreInput(GameManager *game, Assets *assets, SaveData *saveData,
        std::vector<Bird*> player_birds, std::vector<int> player_numbers) : Context(game), saveData(saveData) {
    assert(player_birds.size() == player_numbers.size());

    for (int player_number : player_numbers)
        player_indexes.push_back(player_number - 1);

    std::cout << player_birds.size() << " Player(s) have set a new high score: " << std::endl;
    for (int i = 0; i < player_birds.size(); ++i)
        std::cout << "Player " << player_numbers[i] << " with " << player_birds[i]->score << " points." << std::endl;

    int x, y;
    for (int i = 0; i < player_numbers.size(); ++i) {
        x = (gameManager->ScreenWidth() / 2) * (i % 2);
        y = (gameManager->ScreenHeight() / 2) * (i / 2);
        highScoreWindows.emplace_back(x, y, assets, player_numbers[i], player_birds[i], assets->bird_frames);
        displayed_windows.emplace_back(i < 4);
    }
}

void ContextHighScoreInput::Update(double delta_time, std::vector<Input> controller_inputs) {
    Input* input;
    for (int i = 0; i < player_indexes.size(); ++i) {
        if (!displayed_windows[i] || highScoreWindows[i].IsFinished())
            continue;
        input = &controller_inputs[player_indexes[i]];
        if (input->up_pressed)
            highScoreWindows[i].Up();
        if (input->down_pressed)
            highScoreWindows[i].Down();
        if (input->left_pressed)
            highScoreWindows[i].Left();
        if (input->right_pressed)
            highScoreWindows[i].Right();
        if (input->back_pressed)
            highScoreWindows[i].Backspace();
        if (input->flap_pressed) {
            highScoreWindows[i].Select();
            if (highScoreWindows[i].IsFinished()) {
                completed_windows++;
                if (player_indexes.size() - completed_windows >= 4) {
                    displayed_windows[i] = false;
                    int index = 0;
                    for (; index < player_indexes.size(); ++index)
                        if (!highScoreWindows[index].IsFinished() && displayed_windows[index] == false)
                            break;
                    displayed_windows[index] = true;
                    highScoreWindows[index].SetPosition(highScoreWindows[i].GetPosition());
                }
            }
        }
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

    for (int i = 0; i < highScoreWindows.size(); ++i)
        if (displayed_windows[i])
            highScoreWindows[i].Render(renderer);
}


