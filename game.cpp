#include <utility>
#include <algorithm>
#include "game.hpp"
#include "auxillary.hpp"
#include "contexts/context_main.hpp"
#include "contexts/context_high_score_view.hpp"

const char *GameManager::game_name = "Super Flappy Birds";
const char *GameManager::game_version = "0.9.4";

GameManager::GameManager(unsigned int screen_width, unsigned int screen_height, std::string core_folder_path, std::string config_folder_path, unsigned int max_players, bool show_wins) {
    this->screen_width = screen_width;
    this->screen_height = screen_height;
    framebuffer.resize(screen_width * screen_height);

    assets = new Assets(core_folder_path);

    save_data = new SaveData(std::move(config_folder_path));
    save_data->Deserialize();

    screen = new Renderer(screen_width, screen_height);

    auto context_main = new ContextMain(this, &mixer, screen_width, screen_height, assets, save_data, max_players, show_wins);
    contexts.push(context_main);
}

void GameManager::GameLoop(double delta_time, std::vector<Input> controller_inputs) {
    contexts.top()->Update(delta_time, std::move(controller_inputs));
}

uint32_t* GameManager::GetFrameBuffer() {
    contexts.top()->Render(screen);
    return screen->framebuffer;
}

void GameManager::PushNewContext(Context *context) {
    contexts.push(context);
}

void GameManager::EndCurrentContext() {
    delete contexts.top();
    contexts.pop();
}

unsigned int GameManager::ScreenWidth() {
    return screen_width;
}

unsigned int GameManager::ScreenHeight() {
    return screen_height;
}
