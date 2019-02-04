#ifndef LR_SUPERFLAPPYBIRDS_AUDIO_MIXER_HPP
#define LR_SUPERFLAPPYBIRDS_AUDIO_MIXER_HPP


#include "sound.hpp"

class AudioMixer {
public:
    std::vector<int16_t> buffer;

    void Play(Sound *sound);

    void Render(int16_t *buffer, size_t length);

    void Enable(bool enable);

private:
    bool enabled = true;
};


#endif //LR_SUPERFLAPPYBIRDS_AUDIO_MIXER_HPP
