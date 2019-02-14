#include "libretro.hpp"
#include "game.hpp"
#include <cstring>
#include <memory>

static const unsigned FRAMEBUFFER_WIDTH = 640;
static const unsigned FRAMEBUFFER_HEIGHT = 360;

std::string core_path;
std::string system_dir;
std::vector<Input> input;
std::unique_ptr<GameManager> game;
#define AUDIO_FRAMES (44100 / 60)
int16_t buffer[AUDIO_FRAMES * 2];

static bool use_audio_cb;

// Callbacks
static retro_log_printf_t log_cb;
static retro_video_refresh_t video_cb;
static retro_input_poll_t input_poll_cb;
static retro_input_state_t input_state_cb;
static retro_environment_t environ_cb;
static retro_audio_sample_t audio_cb;
static retro_audio_sample_batch_t audio_batch_cb;

static double delta_time;

struct EnvVariables {
    unsigned int max_players;
    bool show_wins;
};

static EnvVariables get_env_variables() {
    EnvVariables vars{};
    retro_variable var = { "sfb_max_players" };
    if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
    {
        vars.max_players = std::stoul(var.value);
        std::cout << "Config states the max players should be: " << vars.max_players << std::endl;
    } else {
        vars.max_players = 4;
        std::cout << "Config does not have a max players value, defaulting to: " << vars.max_players << std::endl;
    }
    var = { "sfb_show_wins" };
    if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
    {
        vars.show_wins = std::strcmp(var.value, "on") == 0;
        std::cout << "Config states the 'show wins' should be: " << vars.show_wins << std::endl;
    } else {
        vars.show_wins = true;
        std::cout << "Config does not have a 'show wins' value, defaulting to: " << vars.show_wins << std::endl;
    }

    return vars;
}

static bool env_variables_changed() {
    bool update = false;
    return environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE_UPDATE, &update) && update;
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

static void audio_callback(void)
{
    game->mixer.Render(buffer, AUDIO_FRAMES * 2);
    for (size_t i = 0; i < AUDIO_FRAMES; )
    {
        size_t written = audio_batch_cb(buffer + 2 * i, AUDIO_FRAMES - i);
        i += written;
    }
}

static void audio_set_state(bool enable)
{
    game->mixer.Enable(enable);
}

bool retro_load_game(const struct retro_game_info *info)
{
    struct retro_audio_callback cb = { audio_callback, audio_set_state };
    use_audio_cb = environ_cb(RETRO_ENVIRONMENT_SET_AUDIO_CALLBACK, &cb);

    auto env_variables = get_env_variables();
    input = {{}, {}, {}, {}, {}, {}, {}, {}};
    game = std::make_unique<GameManager>(FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT, core_path, system_dir, env_variables.max_players, env_variables.show_wins);
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
    return nullptr;
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
    retro_usec_t time_reference = 1000000 / GameManager::game_fps;
    auto frame_time_cb = [](retro_usec_t usec) { delta_time = usec / 1000000.0; };
    struct retro_frame_time_callback frame_cb = { frame_time_cb, time_reference };
    cb(RETRO_ENVIRONMENT_SET_FRAME_TIME_CALLBACK, &frame_cb);

    // retro_set_environment seems to get called multiple times, but only the first time contains the right path.
    if (core_path.empty()) {
        char *name = new char[PATH_MAX];  // TODO: Memory leak, but I can't delete[] it?
        cb(RETRO_ENVIRONMENT_GET_LIBRETRO_PATH, &name);
        core_path = std::string(name);
        core_path = core_path.substr(0, core_path.find_last_of('/') + 1);
        std::cout << "core path: " << core_path << std::endl;
        cb(RETRO_ENVIRONMENT_GET_SYSTEM_DIRECTORY, &name);
        system_dir = std::string(name);
        if (system_dir.find('/') != std::string::npos)
            system_dir += '/';
        else if (system_dir.find('\\') != std::string::npos)
            system_dir += '\\';
        std::cout << "system dir path: " << system_dir << std::endl;
    }

    get_env_variables();

    // Environment variables
    environ_cb = cb;
    // TODO: Sets the default value to 4, this is very hackish, but there doesn't seem to be a better way.
    retro_variable vars[] = {
            { "sfb_max_players", "Maximum number of players allowed (Requires restart); 4|2|3|1|5|6|7|8" },
            { "sfb_show_wins", "Shows wins in multiplayer game (Requires restart); on|off" },
            { nullptr, nullptr },
    };
    cb(RETRO_ENVIRONMENT_SET_VARIABLES, vars);
    vars[0] = { "sfb_max_players", "Maximum number of players allowed (Requires restart); 1|2|3|4|5|6|7|8" };
    cb(RETRO_ENVIRONMENT_SET_VARIABLES, vars);

    get_env_variables();
}

void retro_set_audio_sample_batch(retro_audio_sample_batch_t cb) { audio_batch_cb = cb; }
void retro_set_video_refresh(retro_video_refresh_t cb) { video_cb = cb; }
void retro_set_audio_sample(retro_audio_sample_t cb) { audio_cb = cb; }
void retro_set_input_poll(retro_input_poll_t cb) { input_poll_cb = cb; }
void retro_set_input_state(retro_input_state_t cb) { input_state_cb = cb; }



void retro_init(void)
{
    struct retro_log_callback log{};
    if (environ_cb(RETRO_ENVIRONMENT_GET_LOG_INTERFACE, &log))
        log_cb = log.log;
    else
        log_cb = nullptr;
    std::cout << "Setting up a maximum of " << input.size() << " controllers." << std::endl;
    std::cout << "Max threads available for this system: " << omp_get_max_threads() << std::endl;
    if (log_cb)
        log_cb(RETRO_LOG_INFO, "SuperFlappyBird: ", "Max threads available for this system: %d.\n", omp_get_max_threads());
}

void retro_get_system_info(struct retro_system_info *info)
{
    memset(info, 0, sizeof(*info));
    info->library_name = GameManager::game_name;
    info->library_version = GameManager::game_version;
    info->need_fullpath = false;
    info->valid_extensions = "zip";
}

void retro_get_system_av_info(struct retro_system_av_info *info)
{

    int pixel_format = RETRO_PIXEL_FORMAT_XRGB8888;

    memset(info, 0, sizeof(*info));
    info->timing.fps            = GameManager::game_fps;
    info->timing.sample_rate    = 44100;
    info->geometry.base_width   = FRAMEBUFFER_WIDTH;
    info->geometry.base_height  = FRAMEBUFFER_HEIGHT;
    info->geometry.max_width    = FRAMEBUFFER_WIDTH;
    info->geometry.max_height   = FRAMEBUFFER_HEIGHT;
    info->geometry.aspect_ratio = (float) FRAMEBUFFER_WIDTH / (float) FRAMEBUFFER_HEIGHT;

    environ_cb(RETRO_ENVIRONMENT_SET_PIXEL_FORMAT, &pixel_format);
}

void retro_reset(void) { }

std::vector<Input> retropad_map_input(unsigned long num_controllers, std::vector<Input> previous_input) {
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