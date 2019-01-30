#include "image.hpp"

uint32_t * Image::ParseASCII(std::basic_istream<char, std::char_traits<char>> &file, int width, int height, int max_value) {
    auto image = new uint32_t[width * height];
    std::string line;
    unsigned int pixel, r, g, b;
    unsigned int line_i = 0;
    while (std::getline(file, line))
    {
        std::istringstream iss(line);
        iss >> pixel;
        switch (line_i % 3) {
            case 0:
                r = pixel;
                break;
            case 1:
                g = pixel;
                break;
            case 2:
                b = pixel;

                image[line_i / 3] = r | g << 8 | b << 16 | 255 << 24;
                break;
        }
        line_i++;
    }
    return image;
}

uint32_t *
Image::ParseBinary(std::basic_istream<char, std::char_traits<char>> &file, int width, int height, int max_value,
                   int channels) {
    assert(channels == 3 || channels == 4);
    auto image = new uint32_t[width * height];
    unsigned int r, g, b, a = 255;
    std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(file), {});
    for (int i = 0; i < buffer.size(); i += channels) {
        r = buffer[i];
        g = buffer[i + 1];
        b = buffer[i + 2];
        if (channels == 4)
            a = buffer[i + 3];
        image[i / channels] = b | g << 8 | r << 16 | a << 24;
    }

    return image;
}

uint32_t *
Image::ParsePPM(std::basic_istream<char, std::char_traits<char>> &file, const std::string &magic_number, int *width,
                int *height) {
    int max_value;

    std::string line;
    unsigned int line_i = 1;
    while (std::getline(file, line))
    {
        if (line[0] == '#')
            continue;

        std::istringstream iss(line);
        switch (line_i++) {
            // width and height
            case 1:
                iss >> *width >> *height;
                break;
                // Maximum value
            case 2:
                iss >> max_value;

                // Data
                if (magic_number == "P3")
                    return ParseASCII(file, *width, *height, max_value);
                return ParseBinary(file, *width, *height, max_value);
        }
    }

    throw "Invalid file format";
}

uint32_t *Image::ParsePAM(std::basic_istream<char, std::char_traits<char>> &file, int *width, int *height) {
    int max_value, depth;
    std::string label, tuple_type;

    std::string line;
    while (std::getline(file, line))
    {
        if (line[0] == '#')
            continue;

        std::istringstream iss(line);
        iss >> label;
        if (label == "WIDTH")
            iss >> *width;
        else if (label == "HEIGHT")
            iss >> *height;
        else if (label == "DEPTH")
            iss >> depth;
        else if (label == "MAXVAL")
            iss >> max_value;
        else if (label == "TUPLTYPE")
            iss >> tuple_type;
        else if (label == "ENDHDR")
            return ParseBinary(file, *width, *height, max_value, depth);
    }

    throw "Invalid file format";
}

Texture *Image::LoadPNM(const std::string &file_path) {
    std::ifstream infile(file_path);
    std::string magic_number;
    int width, height;

    std::string line;
    if (std::getline(infile, line))
    {
        // File format, aka magic number
        if (line == "P3" || line == "P6")
            return new Texture(ParsePPM(infile, line, &width, &height), width, height);
        if (line == "P7")
            return new Texture(ParsePAM(infile, &width, &height), width, height);
        else
            throw "Invalid file format";
    }
    throw "Invalid file format";
}
