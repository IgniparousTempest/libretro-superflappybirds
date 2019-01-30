#ifndef LR_SUPERFLAPPYBIRDS_SETTINGS_HPP
#define LR_SUPERFLAPPYBIRDS_SETTINGS_HPP

#include <fstream>
#include <iostream>

class Settings {
public:
    int highscore = 0;
    std::string filename = "settings.txt";

    explicit Settings(std::string config_folder_path);
    void Serialize();
    void Deserialize();
};


#endif //LR_SUPERFLAPPYBIRDS_SETTINGS_HPP
