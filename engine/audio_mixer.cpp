#include <climits>
#include <algorithm>
#include "audio_mixer.hpp"

void AudioMixer::Play(Sound *sound) {
    std::vector<int16_t> stream = sound->data;
    streams.push_back(stream);
}

void AudioMixer::Render(int16_t *audio_buffer, size_t length) {
    if (enabled) {
        // Combine signals
        int64_t signal;
        for (int i = 0; i < length; ++i) {
            signal = 0;
            for (auto &s : streams) {
                if (i < s.size())
                    signal = CombineStreamSamples(signal, s[i]);
            }
            audio_buffer[i] = ClampSample(signal);
        }

        // Remove empty streams
        streams.erase(std::remove_if(
                streams.begin(), streams.end(),
                [length](const std::vector<int16_t>& x) {
                    return x.size() <= length; // put your condition here
                }), streams.end());

        // Remove rendered chunk from streams
        for (auto &stream : streams) {
            stream.erase(stream.begin(), stream.begin() + length);
        }
    } else {
        for (int i = 0; i < length; ++i)
            audio_buffer[i] = 0;
    }
}

void AudioMixer::Enable(bool enable) {
    enabled = enable;
}

int64_t AudioMixer::CombineStreamSamples(int64_t augend, int64_t addend) {
    if (addend > 0) {
        // Addition
        if (augend > LONG_MAX - addend) {
            // Overflow prevented
            return LONG_MAX;
        } else {
            return augend + addend;
        }
    } else if (addend < 0) {
        // Subtraction
        if (augend < LONG_MIN - addend) {
            // Underflow prevented
            return LONG_MIN;
        } else {
            return augend + addend;
        }
    }
    else {
        // Nothing
        return augend;
    }
}

int16_t AudioMixer::ClampSample(int64_t signal) {
    if (signal < SHRT_MIN)
        return SHRT_MIN;
    else if (signal > SHRT_MAX)
        return SHRT_MAX;
    else
        return static_cast<int16_t>(signal);
}
