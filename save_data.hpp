#ifndef LR_SUPERFLAPPYBIRDS_SETTINGS_HPP
#define LR_SUPERFLAPPYBIRDS_SETTINGS_HPP

#include <fstream>
#include <iostream>
#include <vector>

class SaveData {
public:
    static const int MAX_SCORES = 10;
    std::vector<std::pair<std::string, int>> top_scores;
    std::string filename = "superflappybirds_highscores.dat";

    explicit SaveData(std::string config_folder_path);
    void Serialize();
    void Deserialize();

    /// Adds a new high scorer to the list in the correct order.
    /// \param name The player's name.
    /// \param score The player's score.
    /// \return True if the entry was added, false otherwise.
    bool AddNewScore(std::string name, int score);

    /// Determines how many of the players provided would get onto the high score table.
    /// \param scores The scores for the players, needs to be in order from lowest to highest.
    /// \return The number of players that would be added to the high score table.
    int DoesPlayerQualifyForHighScoreTable(const std::vector<int> &scores);

    /// Gets the highest score in the game so far.
    /// \return The highest score
    int HighScore();
};


#endif //LR_SUPERFLAPPYBIRDS_SETTINGS_HPP
