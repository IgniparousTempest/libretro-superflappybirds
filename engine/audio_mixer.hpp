#ifndef LR_SUPERFLAPPYBIRDS_AUDIO_MIXER_HPP
#define LR_SUPERFLAPPYBIRDS_AUDIO_MIXER_HPP


#include <cstddef>
#include "sound.hpp"

class AudioMixer {
public:
    std::vector<int16_t> buffer;

    /// Starts playing a sound.
    /// \param sound The sound to play.
    void Play(Sound *sound);

    /// Gets the next part of the audio stream to play.
    /// \param buffer Destination to copy audio stream to.
    /// \param length The length of the audio stream to be copied.
    void Render(int16_t *buffer, size_t length);

    /// Enables or disables the sound output.
    /// \param enable Whether to enables or disables the sound output.
    void Enable(bool enable);

private:
    bool enabled = true;
};


#endif //LR_SUPERFLAPPYBIRDS_AUDIO_MIXER_HPP
