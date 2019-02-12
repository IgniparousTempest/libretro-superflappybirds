#ifndef SUPERFLAPPYBIRDS_LIBRETRO_CONTEXT_HPP
#define SUPERFLAPPYBIRDS_LIBRETRO_CONTEXT_HPP

#include <vector>
#include "../engine/renderer.hpp"
#include "../input.hpp"
#include "../game.hpp"

class Context {
public:
    explicit Context(Game *game) {
        gameManager = game;
    }

    virtual void Update(double delta_time, std::vector<Input> controller_inputs) = 0;

    virtual void Render(Renderer *renderer) = 0;

protected:
    Game * gameManager;
};

#endif //SUPERFLAPPYBIRDS_LIBRETRO_CONTEXT_HPP
