#ifndef LR_SUPERFLAPPYBIRDS_SOUND_HPP
#define LR_SUPERFLAPPYBIRDS_SOUND_HPP

#include <cstdint>
#include <vector>
#include <utility>

struct Sound {
    unsigned int num_channels;
    unsigned int sample_rate;
    std::vector<int16_t> data;

    Sound(unsigned int num_channels, unsigned int sample_rate, std::vector<int16_t> data):
    num_channels(num_channels), sample_rate(sample_rate), data(std::move(data)) {}
};

#endif //LR_SUPERFLAPPYBIRDS_SOUND_HPP
