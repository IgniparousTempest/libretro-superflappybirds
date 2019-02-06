#ifndef LR_SUPERFLAPPYBIRDS_AUDIO_MIXER_HPP
#define LR_SUPERFLAPPYBIRDS_AUDIO_MIXER_HPP


#include <cstddef>
#include <deque>
#include "sound.hpp"

class AudioMixer {
public:
    std::deque<std::vector<int16_t>> streams;

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

    /// Result = augend + addend. If overflow or underflow occurs, then it clips to either LONG_MIN or LONG_MAX.
    /// \param augend The number to add to.
    /// \param addend The number you adding to the augend.
    /// \return A clipped form of the summation.
    int64_t CombineStreamSamples(int64_t augend, int64_t addend);

    /// Clamps a long sample to a short sample. I.e. ensures signal is in range [SHRT_MIN, SHRT_MAX].
    /// \param signal The signal to clamp.
    /// \return The clamped input.
    int16_t ClampSample(int64_t signal);
};


#endif //LR_SUPERFLAPPYBIRDS_AUDIO_MIXER_HPP
