#include "settings.hpp"

Settings::Settings(std::string config_folder_path) {
    filename = config_folder_path + filename;
}
void Settings::Serialize() {
    std::ofstream out_file (filename);
    if (out_file.fail())
        std::cerr << "Failed to write saved data at: " << filename << std::endl;
    out_file << highscore << std::endl;
}

void Settings::Deserialize() {
    std::ifstream in_file (filename);
    if (!in_file.fail()) {
        in_file >> highscore;
    }
}
