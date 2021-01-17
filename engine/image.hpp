#ifndef LR_SUPERFLAPPYBIRDS_IMAGE_HPP
#define LR_SUPERFLAPPYBIRDS_IMAGE_HPP

#include <stdexcept>
#include <cstdint>
#include <string>
#include <fstream>
#include <sstream>
#include <cassert>
#include <vector>
#include "texture.hpp"

class Image {
public:
    /// Loads images from the PNM family.
    /// Only the following formats are allowed:
    /// * .ppm (binary and ascii)
    /// * .pam (binary)
    /// \param file_path The path to the image file.
    /// \return A texture.
    static Texture* LoadPNM(const std::string &file_path);

private:
    static uint32_t* ParseASCII(std::basic_istream<char, std::char_traits<char>> &file, int width, int height, int max_value);

    static uint32_t* ParseBinary(std::basic_istream<char, std::char_traits<char>> &file, int width, int height, int max_value, int channels = 3);

    static uint32_t* ParsePPM(std::basic_istream<char, std::char_traits<char>>& file, const std::string &magic_number, int* width, int* height);

    static uint32_t* ParsePAM(std::basic_istream<char, std::char_traits<char>>& file, int* width, int* height);
};

#endif //LR_SUPERFLAPPYBIRDS_IMAGE_HPP
