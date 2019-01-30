#include "libretro.hpp"
#include "game.hpp"
#include "input.hpp"
#include <cstring>
#include <memory>
#include <iostream>
#include <dlfcn.h>
#include <libgen.h>

static const unsigned FRAMEBUFFER_WIDTH = 640;
static const unsigned FRAMEBUFFER_HEIGHT = 360;

std::vector<Input> input = {{}, {}, {}, {}};
std::unique_ptr<Game> game;

// Callbacks
static retro_log_printf_t log_cb;
static retro_video_refresh_t video_cb;
static retro_input_poll_t input_poll_cb;
static retro_input_state_t input_state_cb;
static retro_environment_t environ_cb;
static retro_audio_sample_t audio_cb;
static retro_audio_sample_batch_t audio_batch_cb;

static double delta_time;

std::string get_library_path() {
    Dl_info dl_info;
    if(0 == dladdr((void*)get_library_path, &dl_info))
        return std::string(dl_info.dli_fname);
    else
        return std::string();
}

unsigned retro_api_version(void)
{
    return RETRO_API_VERSION;
}

void retro_cheat_reset(void)
{

}

void retro_cheat_set(unsigned index, bool enabled, const char *code)
{

}

bool retro_load_game(const struct retro_game_info *info)
{
    return true;
}

bool retro_load_game_special(unsigned game_type, const struct retro_game_info *info, size_t num_info)
{
    return false;
}

void retro_unload_game(void)
{

}

unsigned retro_get_region(void)
{
    return RETRO_REGION_NTSC;
}

void retro_set_controller_port_device(unsigned port, unsigned device)
{

}

void *retro_get_memory_data(unsigned id)
{
    return NULL;
}

size_t retro_get_memory_size(unsigned id)
{
    return 0;
}

size_t retro_serialize_size(void)
{
    return 0;
}

bool retro_serialize(void *data, size_t size)
{
    return false;
}

bool retro_unserialize(const void *data, size_t size)
{
    return false;
}

void retro_deinit(void)
{
}

void retro_set_environment(retro_environment_t cb)
{
    environ_cb = cb;
    // Start without rom
    bool no_rom = true;
    cb(RETRO_ENVIRONMENT_SET_SUPPORT_NO_GAME, &no_rom);
    // Delta time setup
    retro_usec_t time_reference = 1000000 / game->game_fps;
    auto frame_time_cb = [](retro_usec_t usec) { delta_time = usec / 1000000.0; };
    struct retro_frame_time_callback frame_cb = { frame_time_cb, time_reference };
    cb(RETRO_ENVIRONMENT_SET_FRAME_TIME_CALLBACK, &frame_cb);
}

void retro_set_audio_sample_batch(retro_audio_sample_batch_t cb) { audio_batch_cb = cb; }
void retro_set_video_refresh(retro_video_refresh_t cb) { video_cb = cb; }
void retro_set_audio_sample(retro_audio_sample_t cb) { audio_cb = cb; }
void retro_set_input_poll(retro_input_poll_t cb) { input_poll_cb = cb; }
void retro_set_input_state(retro_input_state_t cb) { input_state_cb = cb; }



void retro_init(void)
{
    //TODO: RETRO_ENVIRONMENT_GET_LIBRETRO_PATH seems to return gibberish.
    auto path = dirname(get_library_path());
    std::cout << path << std::endl;
    game = std::make_unique<Game>(FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT, path);
}

void retro_get_system_info(struct retro_system_info *info)
{
    memset(info, 0, sizeof(*info));
    info->library_name = game->game_name;
    info->library_version = game->game_version;
    info->need_fullpath = false;
    info->valid_extensions = "zip";
}

void retro_get_system_av_info(struct retro_system_av_info *info)
{

    int pixel_format = RETRO_PIXEL_FORMAT_XRGB8888;

    memset(info, 0, sizeof(*info));
    info->timing.fps            = game->game_fps;
    info->timing.sample_rate    = 44100;
    info->geometry.base_width   = FRAMEBUFFER_WIDTH;
    info->geometry.base_height  = FRAMEBUFFER_HEIGHT;
    info->geometry.max_width    = FRAMEBUFFER_WIDTH;
    info->geometry.max_height   = FRAMEBUFFER_HEIGHT;
    info->geometry.aspect_ratio = (float) FRAMEBUFFER_WIDTH / (float) FRAMEBUFFER_HEIGHT;

    environ_cb(RETRO_ENVIRONMENT_SET_PIXEL_FORMAT, &pixel_format);
}

void retro_reset(void) { }

std::vector<Input> retropad_map_input(unsigned int num_controllers, std::vector<Input> previous_input) {
    // Poll input
    input_poll_cb();

    std::vector<Input> controller_states;
    for (unsigned int player = 0; player < num_controllers; ++player) {
        Input input = {};

        input.flap = input_state_cb(player, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_A) == 1;
        input.back = input_state_cb(player, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_B) == 1;
        input.right = input_state_cb(player, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT) == 1;
        input.down = input_state_cb(player, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN) == 1;
        input.left = input_state_cb(player, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT) == 1;
        input.up = input_state_cb(player, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP) == 1;
        input.start = input_state_cb(player, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_START) == 1;

        input.flap_pressed = !previous_input[player].flap && input.flap;
        input.back_pressed = !previous_input[player].back && input.back;
        input.right_pressed = !previous_input[player].right && input.right;
        input.down_pressed = !previous_input[player].down && input.down;
        input.left_pressed = !previous_input[player].left && input.left;
        input.up_pressed = !previous_input[player].up && input.up;
        input.start_pressed = !previous_input[player].start && input.start;

        input.flap_released = previous_input[player].flap && !input.flap;
        input.back_released = previous_input[player].back && !input.back;
        input.right_released = previous_input[player].right && !input.right;
        input.down_released = previous_input[player].down && !input.down;
        input.left_released = previous_input[player].left && !input.left;
        input.up_released = previous_input[player].up && !input.up;
        input.start_released = previous_input[player].start && !input.start;

        controller_states.push_back(input);
    }
    return controller_states;
}


void retro_run(void)
{
    // Input
    input = retropad_map_input(input.size(), input);

    game->GameLoop(delta_time, input);

    video_cb(game->GetFrameBuffer(), FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT, sizeof(uint32_t) * FRAMEBUFFER_WIDTH);
}