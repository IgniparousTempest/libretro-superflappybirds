#include <algorithm>
#include <sstream>
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
    for (auto &top_score : top_scores)
        out_file << top_score.first << "," << top_score.second << std::endl;
}

void SaveData::Deserialize() {
    std::string name;
    int score;
    std::ifstream in_file (filename);
    if (!in_file.fail()) {
        top_scores = {};
        std::string line, value;
        while(getline(in_file, line))
        {
            std::stringstream linestream(line);
            getline(linestream, value, ',');
            name = value;
            getline(linestream, value, ',');
            score = std::stoi(value);
            AddNewScore(name, score);
        }
    }
}

bool SaveData::AddNewScore(std::string name, int score) {
    // Don't add score if the list is full and the new score is too low.
    if (top_scores.size() >= MAX_SCORES && score <= top_scores.back().second)
        return false;

    top_scores.emplace_back(name, score);

    // Re-order the scores
    std::sort(top_scores.begin(), top_scores.end(), [](auto &left, auto &right) {
        return left.second > right.second;
    });

    while (top_scores.size() > MAX_SCORES)
        top_scores.pop_back();

    return true;
}

int SaveData::DoesPlayerQualifyForHighScoreTable(const std::vector<int> &scores) {
    SaveData data("");
    data.top_scores = this->top_scores;
    for (int i = 0; i < scores.size(); ++i) {
        if (!data.AddNewScore("", scores[i]))
            return i + 1;
    }
    return static_cast<int>(scores.size());
}

int SaveData::HighScore() {
    return top_scores.empty() ? 0 : top_scores.front().second;
}
