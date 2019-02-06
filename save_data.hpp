#ifndef LR_SUPERFLAPPYBIRDS_SETTINGS_HPP
#define LR_SUPERFLAPPYBIRDS_SETTINGS_HPP

#include <fstream>
#include <iostream>

class SaveData {
public:
    int highscore = 0;
    std::string filename = "superflappybirds_highscore.dat";

    explicit SaveData(std::string config_folder_path);
    void Serialize();
    void Deserialize();
};


#endif //LR_SUPERFLAPPYBIRDS_SETTINGS_HPP
