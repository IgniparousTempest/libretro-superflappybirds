#include "audio_mixer.hpp"

void AudioMixer::Play(Sound *sound) {
    if (buffer.size() < sound->data.size())
        buffer.resize(sound->data.size());

    for (int i = 0; i < sound->data.size(); ++i)
        buffer[i] = sound->data[i];
}

void AudioMixer::Render(int16_t *audio_buffer, size_t length) {
    if (enabled) {
        for (int i = 0; i < length; ++i)
            if (i < buffer.size())
                audio_buffer[i] = buffer[i];
            else
                audio_buffer[i] = 0;

        if (buffer.size() >= length)
            buffer.erase(buffer.begin(), buffer.begin() + length);
        else
            buffer.clear();
    } else {
        for (int i = 0; i < length; ++i)
            audio_buffer[i] = 0;
    }
}

void AudioMixer::Enable(bool enable) {
    enabled = enable;
}
