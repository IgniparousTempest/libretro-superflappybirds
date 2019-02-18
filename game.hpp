#ifndef LR_SUPERFLAPPYBIRD_GAME_H
#define LR_SUPERFLAPPYBIRD_GAME_H

#include <stdint.h>
#include <vector>
#include <stack>
#include "assets.hpp"
#include "input.hpp"
#include "save_data.hpp"
#include "engine/audio_mixer.hpp"
#include "contexts/context.hpp"

class Context;

class GameManager {
public:
    static const char *game_name;
    static const char *game_version;
    static const int game_fps = 50;

    AudioMixer mixer;

    GameManager(unsigned int screen_width, unsigned int screen_height, std::string core_folder_path, std::string config_folder_path, unsigned int max_players, bool show_wins);

    void GameLoop(double delta_time, std::vector<Input> controller_inputs);

    uint32_t *GetFrameBuffer();

    void PushNewContext(Context *context);

    void EndCurrentContext();

    unsigned int ScreenWidth();

    unsigned int ScreenHeight();

private:
    std::stack<Context*> contexts;
    Renderer *screen;
    SaveData *save_data;
    Assets *assets;
    std::vector<uint32_t> framebuffer;
    unsigned int screen_width;
    unsigned int screen_height;
};


#endif //LR_SUPERFLAPPYBIRD_GAME_H
