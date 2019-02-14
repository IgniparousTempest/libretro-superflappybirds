#ifndef SUPERFLAPPYBIRDS_LIBRETRO_CONTEXT_HPP
#define SUPERFLAPPYBIRDS_LIBRETRO_CONTEXT_HPP

#include <vector>
#include "../engine/renderer.hpp"
#include "../input.hpp"
#include "../game.hpp"

class GameManager;

class Context {
public:
    explicit Context(GameManager *game) {
        gameManager = game;
    }

    virtual ~Context() {}

    virtual void Update(double delta_time, std::vector<Input> controller_inputs) = 0;

    virtual void Render(Renderer *renderer) = 0;

protected:
    GameManager *gameManager;
};

#endif //SUPERFLAPPYBIRDS_LIBRETRO_CONTEXT_HPP
