#include <utility>

#ifndef LR_SUPERFLAPPYBIRDS_SETTINGS_HPP
#define LR_SUPERFLAPPYBIRDS_SETTINGS_HPP

#include <chrono>
#include <fstream>
#include <iostream>
#include <vector>

struct HighScoreRow {
    std::string name;
    int score;
    std::chrono::time_point<std::chrono::system_clock> time;

    HighScoreRow(std::string name, int score, std::chrono::time_point<std::chrono::system_clock> time) :
    name(std::move(name)), score(score), time(time) {}
};

class SaveData {
public:
    static const int MAX_SCORES = 10;
    std::vector<HighScoreRow> top_scores;
    std::string filename = "superflappybirds_highscores.dat";

    explicit SaveData(std::string config_folder_path);
    void Serialize();
    void Deserialize();

    /// Adds a new high scorer to the list in the correct order.
    /// \param name The player's name.
    /// \param score The player's score.
    /// \param time The time the player made the high score.
    /// \return True if the entry was added, false otherwise.
    bool AddNewScore(std::string name, int score, std::chrono::time_point<std::chrono::system_clock> *time = nullptr);

    /// Determines how many of the players provided would get onto the high score table.
    /// \param scores The scores for the players, needs to be in order from lowest to highest.
    /// \return The number of players that would be added to the high score table.
    int DoesPlayerQualifyForHighScoreTable(const std::vector<int> &scores);

    /// Gets the highest score in the game so far.
    /// \return The highest score
    int HighScore();
};


#endif //LR_SUPERFLAPPYBIRDS_SETTINGS_HPP
