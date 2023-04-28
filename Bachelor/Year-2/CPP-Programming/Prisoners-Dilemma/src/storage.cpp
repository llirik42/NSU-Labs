#include <fstream>
#include <regex>
#include <vector>
#include "storage.h"

inline const std::string HISTORY_FILE_NAME = "history";

inline bool check_history_line(const std::string& history_line){
    static const std::regex regex(R"(((\w+):[CD] ){3})");
    return std::regex_match(history_line, regex);
}

inline Round extract_round_from_history_line(const std::string& string){
    Round result;

    static const std::regex strategy_step_regex(R"(([a-zA-Z]+):[CD])");

    std::smatch smatch;

    std::string string_copy = string;

    std::vector<std::string> substrings_vector; // Contains "Cooperator:C", "Defector:D" etc.

    while (regex_search(string_copy, smatch, strategy_step_regex)){
        substrings_vector.push_back(smatch[0].str());
        string_copy = smatch.suffix();
    }

    for (const auto& s : substrings_vector){
        std::string strategy_name;

        for (size_t i = 0; i < s.size() - 2; i++){
            strategy_name.push_back(s[i]);
        }

        result[strategy_name] = s[s.size() - 1];
    }

    return result;
}

bool read_history(const std::string& configs_path, History& history){
    try{
        std::ifstream ifstream(configs_path + HISTORY_FILE_NAME);

        if (!ifstream.is_open()){
            return false;
        }

        while (!ifstream.eof()){
            std::string current_line;
            std::getline(ifstream, current_line);

            if (!check_history_line(current_line)){
                return false;
            }

            Round current_round = extract_round_from_history_line(current_line);

            history.push_back(current_round);
        }

        return true;
    }
    catch(...){
        throw std::runtime_error("Error while reading history if previous games");
    }
}

/*
bool AbstractStorage::is_current_game_history_empty() const{
    return false;
}
*/

Round AbstractStorage::get_previous_round_info() const{
    return {};
}

History AbstractStorage::get_previous_games_history() const{
    return {};
}

void AbstractStorage::append_round([[maybe_unused]] const Round& round) {}

Storage::Storage(const std::string& configs_path):
    _configs_path(configs_path + '/'){
    // Even if configs_path ends with '/', no error will be occurred.
    // For example, "../configs/history" and "../configs//history" are the same paths for file streamers
    // In case if we don't add '/', then error might occur, because "../configs/history" and "../configshistory" aren't the same

    read_history(_configs_path, _previous_games_history);
}

bool Storage::is_current_game_history_empty() const{
    return _current_game_history.empty();
}

Round Storage::get_previous_round_info() const{
    if (_current_game_history.empty()){
        return {};
    }

    return _current_game_history.back();
}

History Storage::get_previous_games_history() const{
    return _previous_games_history;
}

void Storage::append_round(const Round& round){
    _current_game_history.push_back(round);
}

void Storage::dump_history(){
    try{
        std::ofstream ofstream(_configs_path + HISTORY_FILE_NAME, std::ios::app);

        for (const auto& record : _current_game_history){
            for (const auto& [strategy_name, step] : record){
                ofstream << strategy_name << ':' << step << ' ';
            }
            ofstream.put('\n');
        }
    }
    catch(...){
        throw std::runtime_error("Error while dumping history of current game");
    }
}
