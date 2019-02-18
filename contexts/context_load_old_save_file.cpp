#include "context_load_old_save_file.hpp"

ContextLoadOldSaveFile::ContextLoadOldSaveFile(GameManager *game, Assets *assets, SaveData *save_data, std::string file_path) :
Context(game, "Load old save file context"), assets(assets), save_data(save_data), save_file_path(file_path) {
    std::ifstream save_file(file_path);
    if (save_file.fail()) {
        std::cout << "No old format save file detected." << std::endl;
        finished = true;
        return;
    }
    save_file >> score;
    if (!save_data->DoesPlayerQualifyForHighScoreTable(score)) {
        std::cout << "Old format save file contains a low score." << score << std::endl;
        finished = true;
        return;
    }

    font = assets->font_highscore;

    auto title_src_rects = assets->GetFontSrcRect("PREVIOUS SAVE FILE DETECTED...");
    title_rects = Auxillary::getFontRects(title_src_rects, 0, 20, 3, 4);
    int width = title_rects.back().second.x + title_rects.back().second.w;
    for (auto &rects : title_rects)
        rects.second.x += (gameManager->ScreenWidth() - width) / 2;

    int y = title_rects.back().second.y + title_rects.back().second.h;
    auto subtitle_src_rects = assets->GetFontSrcRect("PLEASE ENTER THE TOP SCORER'S NAME:");
    auto subtitle_rects = Auxillary::getFontRects(subtitle_src_rects, 0, y + 5, 3, 3);
    width = subtitle_rects.back().second.x + subtitle_rects.back().second.w;
    for (auto &rects : subtitle_rects)
        rects.second.x += (gameManager->ScreenWidth() - width) / 2;
    for (auto &rects : subtitle_rects)
        title_rects.push_back(rects);

    int x = gameManager->ScreenWidth() / 2 - assets->highscore_frame->w / 2;
    y = subtitle_rects.back().second.y + subtitle_rects.back().second.h + 20;
    Bird bird(0, 0, 0, assets->bird, assets->bird_frames);
    bird.score = score;
    high_score_sub_window = new HighScoreSubWindow(x, y, assets, 1, &bird, assets->bird_frames);

    y += assets->highscore_frame->h + 50;
    auto instructions_src_rects = assets->GetFontSrcRect("Enter your name to save the score. or no name to delete it.");
    auto instructions_rects = Auxillary::getFontRects(instructions_src_rects, 0, y, 2, 2);
    width = instructions_rects.back().second.x + instructions_rects.back().second.w;
    for (auto &rects : instructions_rects)
        rects.second.x += (gameManager->ScreenWidth() - width) / 2;
    for (auto &rects : instructions_rects)
        title_rects.push_back(rects);
}

void ContextLoadOldSaveFile::Update(double delta_time, std::vector<Input> controller_inputs) {
    if (finished) {
        EndContext();
        return;
    }

    Input *input = &controller_inputs[0];
    if (input->up_pressed)
        high_score_sub_window->Up();
    if (input->down_pressed)
        high_score_sub_window->Down();
    if (input->left_pressed)
        high_score_sub_window->Left();
    if (input->right_pressed)
        high_score_sub_window->Right();
    if (input->back_pressed)
        high_score_sub_window->Backspace();
    if (input->flap_pressed) {
        high_score_sub_window->Select();
        if (high_score_sub_window->IsFinished()) {
            auto name = high_score_sub_window->Name();
            auto score = high_score_sub_window->Score();
            std::cout << "Added old high score: Name=\"" << name << "\", Value=" << score << "." << std::endl;
            if (!name.empty()) {
                save_data->AddNewScore(name, high_score_sub_window->Score());
                save_data->Serialize();
            }
            delete high_score_sub_window;
            EndContext();
            return;
        }
    }
}

void ContextLoadOldSaveFile::Render(Renderer *renderer) {
    // Draw Sky
    renderer->Clear(Renderer::rgb(115, 183, 196));

    for (auto &rect : title_rects)
        renderer->Render(font, &rect.first, &rect.second);

    high_score_sub_window->Render(renderer);
}

void ContextLoadOldSaveFile::EndContext() {
    remove(save_file_path.c_str());
    gameManager->EndCurrentContext();
}
