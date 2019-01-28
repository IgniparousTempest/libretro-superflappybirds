#ifndef LR_SUPERFLAPPYBIRDS_BOX_H
#define LR_SUPERFLAPPYBIRDS_BOX_H

#include <cstdint>
#include "engine/rect.hpp"
#include "engine/renderer.hpp"


class Box {
public:
    double x;
    double y;
    double angle;
    double speed = 20;
    uint32_t* texture;
    Rect* rect;

    Box(int x, int y, double angle) {
        this->angle = angle;
        texture = new uint32_t[20 * 40];
        for (int i = 0; i < 20*40; ++i) {
            texture[i] = Renderer::rgb(0, 255, 0);
        }
        this->x = x;
        this->y = y;
        rect = new Rect(x, y, 20, 40);
    }

    void Update(double delta_time) {
        double moved = speed * delta_time;
        x += std::cos(angle);
        y += std::sin(angle);
        rect->x += (int)x;
        rect->y += (int)y;
    }

    void Render(Renderer* screen) {
        screen->Render(texture, rect);
    }
};

#endif //LR_SUPERFLAPPYBIRDS_BOX_H
