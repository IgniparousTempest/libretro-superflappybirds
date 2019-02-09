#ifndef SUPERFLAPPYBIRDS_LIBRETRO_ANIMATION_HPP
#define SUPERFLAPPYBIRDS_LIBRETRO_ANIMATION_HPP

#include <vector>
#include <stdexcept>
#include "rect.hpp"
#include "texture.hpp"

struct Animation {
    Texture *texture;
    std::vector<Rect> frames;
    std::vector<int> frame_durations;
    int frame_index = 0;
    int elapsed_frames = 0;
    bool animating = true;

    Animation(Texture *texture, std::vector<Rect> frames, std::vector<int> frame_durations) :
    texture(texture), frames(std::move(frames)), frame_durations(std::move(frame_durations)) {
        if (frames.size() != frame_durations.size())
            throw std::runtime_error(
                    "frames (" + std::to_string(frames.size()) + ") is not the same length as frame_durations (" +
                    std::to_string(frame_durations.size()) + ")");
    }

    Animation(Texture *texture, std::vector<Rect> frames, int frame_duration) :
            texture(texture), frames(std::move(frames)) {
        frame_durations = std::vector<int>(frames.size());
        for (int i = 0; i < this->frames.size(); ++i)
            frame_durations.push_back(frame_duration);
    }

    void Update() {
        if (animating && ++elapsed_frames > frame_durations[frame_index]) {
            elapsed_frames = 0;
            if (++frame_index >= frames.size())
                frame_index = 0;
        }
    }

    void Pause() {animating = false;}

    void Resume() {animating = true;}

    Rect *CurrentFrame() {
        return &frames[frame_index];
    }
};

#endif //SUPERFLAPPYBIRDS_LIBRETRO_ANIMATION_HPP