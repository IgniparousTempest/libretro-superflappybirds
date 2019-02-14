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
        out_file << top_score.score << "," << top_score.name << "," << top_score.time.time_since_epoch().count() << std::endl;
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
            score = std::stoi(value);
            getline(linestream, value, ',');
            name = value;
            getline(linestream, value, ',');
            std::chrono::time_point<std::chrono::system_clock> time(std::chrono::seconds(std::stoll(value)));
            AddNewScore(name, score, &time);
        }
    }
}

bool SaveData::AddNewScore(std::string name, int score, std::chrono::time_point<std::chrono::system_clock> *time) {
    // Don't add score if the list is full and the new score is too low.
    if (top_scores.size() >= MAX_SCORES && score <= top_scores.back().score)
        return false;
    if (time == nullptr) {
        auto now = std::chrono::system_clock::now();
        auto time_in_seconds = std::chrono::seconds (now.time_since_epoch().count());
        auto time_now = std::chrono::time_point<std::chrono::system_clock>(time_in_seconds);
        top_scores.emplace_back(name, score, time_now);
    }
    else
        top_scores.emplace_back(name, score, *time);

    // Re-order the scores
    std::sort(top_scores.begin(), top_scores.end(), [](auto &left, auto &right) {
        return left.score > right.score;
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
    return top_scores.empty() ? 0 : top_scores.front().score;
}
