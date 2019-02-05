#ifndef LR_SUPERFLAPPYBIRDS_AUDIO_HPP
#define LR_SUPERFLAPPYBIRDS_AUDIO_HPP

#include <cstdint>
#include <string>
#include "sound.hpp"

struct RiffHeader {
    char chunk_id[4];
    uint32_t chunk_size;
    char format[4];
};

struct FormatSubChunk {
    char subchunk1_id[4];
    uint32_t subchunk1_size;
    uint16_t audio_format;
    uint16_t num_chanels;
    uint32_t sample_rate;
    uint32_t byte_rate;
    uint16_t block_align;
    uint16_t bits_per_sample;
    uint16_t extra_param_size;
};

struct DataSubChunk {
    char subchunk2_id[4];
    uint32_t subchunk2_size;
};

struct WavFile {
    char chunk_id[4];
    uint32_t chunk_size;
    char format[4];
    char subchunk1_id[4];
    uint32_t subchunk1_size;
    uint16_t audio_format;
    uint16_t num_chanels;
    uint32_t sample_rate;
    uint32_t byte_rate;
    uint16_t block_align;
    uint16_t bits_per_sample;
    uint16_t extra_param_size;
    char subchunk2_id[4];
    uint32_t subchunk2_size;
    int16_t* data;
};

class Audio {
public:
    /// Loads a wav file from disk.
    /// \param file_path The path to the wav file.
    /// \return The wav as a sound object.
    static Sound *LoadWav(const std::string &file_path);

private:
    /// Prints a description of the Wav file.
    /// \param wav The wav file to describe.
    static void ExplainWavFile(WavFile wav, bool data = false);
};


#endif //LR_SUPERFLAPPYBIRDS_AUDIO_HPP
