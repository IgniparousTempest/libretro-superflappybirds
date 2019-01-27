#ifndef LR_SUPERFLAPPYBIRDS_SETTINGS_HPP
#define LR_SUPERFLAPPYBIRDS_SETTINGS_HPP

#include <fstream>
#include <iostream>

class Settings {
public:
    int highscore;
    const std::string filename = "settings.txt";

    void Serialize();
    void Deserialize();
};


#endif //LR_SUPERFLAPPYBIRDS_SETTINGS_HPP
