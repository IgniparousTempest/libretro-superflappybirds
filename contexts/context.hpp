#ifndef SUPERFLAPPYBIRDS_LIBRETRO_CONTEXT_HPP
#define SUPERFLAPPYBIRDS_LIBRETRO_CONTEXT_HPP

#include <vector>
#include "../engine/renderer.hpp"
#include "../input.hpp"
#include "../game.hpp"

class GameManager;

class Context {
public:
    explicit Context(GameManager *game, std::string context_name) : gameManager(game), context_name(context_name) {}

    virtual ~Context() {}

    virtual void Update(double delta_time, std::vector<Input> controller_inputs) = 0;

    virtual void Render(Renderer *renderer) = 0;

    std::string Name() {
        return context_name;
    }

protected:
    GameManager *gameManager;
    std::string context_name;
};

#endif //SUPERFLAPPYBIRDS_LIBRETRO_CONTEXT_HPP
