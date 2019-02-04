#include <cstring>
#include <fstream>
#include <iostream>
#include "audio.hpp"

Sound *Audio::LoadWav(const std::string &file_path) {
    try {
        std::ifstream input_file(file_path, std::ios::binary);

        if (input_file.fail())
            throw std::runtime_error("File not found: " + file_path);

        // RIFF Header
        RiffHeader riff_header{};
        input_file.read((char*)&riff_header, sizeof(riff_header));

        if (strncmp(riff_header.chunk_id, "RIFF", 4) != 0 || strncmp(riff_header.format, "WAVE", 4) != 0)
            throw std::runtime_error("Invalid RIFF Header");

        // Format Sub-Chunk
        FormatSubChunk format_sub_chunk{};
        input_file.read((char*)&format_sub_chunk, sizeof(format_sub_chunk));

        if (format_sub_chunk.subchunk1_size == 16)
            throw std::runtime_error("Need to rewind 2 bytes before continuing.");
        else if (!(format_sub_chunk.subchunk1_size == 18 && format_sub_chunk.extra_param_size == 0))
            throw std::runtime_error("Can't process extra parameters.");
        if (format_sub_chunk.bits_per_sample != 16)
            throw std::runtime_error("This parser can only read 16 bit samples.");

        // Data Sub-Chunk
        input_file.seekg(-2, std::ifstream::cur);
        DataSubChunk data_sub_chunk{};
        input_file.read((char*)&data_sub_chunk, sizeof(data_sub_chunk));

        if (strncmp(data_sub_chunk.subchunk2_id, "data", 4) != 0)
            throw std::runtime_error("Invalid DATA Header");

        // Assemble Wav File
        WavFile wav{};
        strncpy(wav.chunk_id, riff_header.chunk_id, 4);
        wav.chunk_size = riff_header.chunk_size;
        strncpy(wav.format, riff_header.format, 4);

        strncpy(wav.subchunk1_id, format_sub_chunk.subchunk1_id, 4);
        wav.subchunk1_size = format_sub_chunk.subchunk1_size;
        wav.audio_format = format_sub_chunk.audio_format;
        wav.num_chanels = format_sub_chunk.num_chanels;
        wav.sample_rate = format_sub_chunk.sample_rate;
        wav.byte_rate = format_sub_chunk.byte_rate;
        wav.block_align = format_sub_chunk.block_align;
        wav.bits_per_sample = format_sub_chunk.bits_per_sample;
        wav.extra_param_size = format_sub_chunk.extra_param_size;

        strncpy(wav.subchunk2_id, data_sub_chunk.subchunk2_id, 4);
        wav.subchunk2_size = data_sub_chunk.subchunk2_size;
        wav.data = new int16_t[wav.subchunk2_size];
        input_file.read((char*)wav.data, wav.subchunk2_size);

        std::vector<int16_t> data;
        data.reserve(wav.subchunk2_size);
        for (int i = 0; i < wav.subchunk2_size; ++i)
            data.push_back(wav.data[i]);
        return new Sound(wav.num_chanels, wav.sample_rate, data);
    }
    catch( const std::exception & ex ) {
        std::cerr << ex.what() << std::endl;
        throw ex;
    }
}

void Audio::ExplainWavFile(WavFile wav) {
    std::cout << "ChunkID:        " << std::string(wav.chunk_id, 4) << "; should be 'RIFF'." << std::endl;
    std::cout << "ChunkSize:      " << wav.chunk_size << " bytes; should be size of file in bytes -8 bytes for the first two fields." << std::endl;
    std::cout << "Format:         " << std::string(wav.format, 4) << "; should be 'WAVE'." << std::endl;
    std::cout << "Subchunk1ID:    " << std::string(wav.subchunk1_id, 4) << "; should be 'fmt '." << std::endl;
    std::cout << "Subchunk1Size:  " << wav.subchunk1_size << "; This is the size of the rest of the Subchunk which follows this number." << std::endl;
    std::cout << "AudioFormat:    " << wav.audio_format << "; PCM = 1 (i.e. Linear quantization), values other than 1 indicate some form of compression." << std::endl;
    std::cout << "NumChannels:    " << wav.num_chanels << "; Mono = 1, Stereo = 2, etc." << std::endl;
    std::cout << "SampleRate:     " << wav.sample_rate << " Hz; 8000, 44100, etc." << std::endl;
    std::cout << "ByteRate:       " << wav.byte_rate << "; = SampleRate * NumChannels * BitsPerSample/8" << std::endl;
    std::cout << "BlockAlign:     " << wav.block_align << "; = NumChannels * BitsPerSample/8. The number of bytes for one sample including all channels." << std::endl;
    std::cout << "BitsPerSample:  " << wav.bits_per_sample << "; 8 bits = 8, 16 bits = 16, etc." << std::endl;
    std::cout << "ExtraParamSize: " << wav.extra_param_size << " bytes; The size of the subsequent ExtraParams chunk." << std::endl;
    std::cout << "Subchunk2ID:    " << std::string(wav.subchunk2_id, 4) << "; should be 'data'." << std::endl;
    std::cout << "Subchunk2Size:  " << wav.subchunk2_size << "; = NumSamples * NumChannels * BitsPerSample/8. Number of samples in data (i.e. length od data[])." << std::endl;
    std::cout << "Data:           " << wav.data << "; The actual sound data." << std::endl;
}
