#include "save_data.hpp"

SaveData::SaveData(std::string config_folder_path) {
    filename = config_folder_path + filename;
}

void SaveData::Serialize() {
    std::ofstream out_file (filename);
    if (out_file.fail())
        std::cerr << "Failed to write save data at: " << filename << std::endl;
    else
        std::cerr << "Writing save data to: " << filename << std::endl;
    out_file << highscore << std::endl;
}

void SaveData::Deserialize() {
    std::ifstream in_file (filename);
    if (!in_file.fail()) {
        in_file >> highscore;
    }
}
