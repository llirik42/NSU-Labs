#include <iostream>
#include "game.h"
#include "utils.h"
#include "viewer.h"

enum class CommandReadingStatus{
    QUIT_COMMAND,
    INCORRECT_COMMAND,
    NO_COMMAND
};

CommandReadingStatus read_command(){
    std::string current_command;
    std::getline(std::cin, current_command);

    if (current_command == "quit"){
        return CommandReadingStatus::QUIT_COMMAND;
    }

    if (!current_command.empty()){
        return CommandReadingStatus::INCORRECT_COMMAND;
    }

    return CommandReadingStatus::NO_COMMAND;
}

void add_score(const Score& source, Score& destination, StrategiesIndexes indexes){
    destination[indexes[0]] += source[0];
    destination[indexes[1]] += source[1];
    destination[indexes[2]] += source[2];
}
void add_score(const Row& source, Score& destination){
    destination[0] += source[0];
    destination[1] += source[1];
    destination[2] += source[2];
}

StrategiesTriplet Game::extract_strategies_triplet(const StrategiesIndexes& indexes) const{
    return StrategiesTriplet({_strategies[indexes[0]], _strategies[indexes[1]], _strategies[indexes[2]]});
}

Score Game::competition(const StrategiesTriplet& strategies_triplet) const{
    Score score({0, 0, 0});

    for (unsigned int i = 0; i < _steps_count; i++){
        if (_mode == GameMode::DETAILED){
            CommandReadingStatus status = read_command();

            if (status == CommandReadingStatus::QUIT_COMMAND){
                break;
            }
            if (status == CommandReadingStatus::INCORRECT_COMMAND){
                GameViewer::view_incorrect_command();
            }
        }

        Choices current_choices;
        Round current_round;

        Round previous_round = {};
        if (!_storage.is_current_game_history_empty()){
            previous_round = _storage.get_previous_round_info();
        }

        for (const auto& current_strategy : strategies_triplet){
            Step current_step = current_strategy->act(previous_round);

            current_choices.push_back(current_step);

            current_round[current_strategy->get_strategy_name()] = current_step;
        }

        _storage.append_round(current_round);

        auto delta_score = _matrix[current_choices];
        add_score(delta_score, score);

        if (_mode == GameMode::DETAILED){
            GameViewer::view_round(score, current_choices, delta_score);
        }
    }

    return score;
}

Score Game::tournament(){
    Score score({0});
    score.resize(_strategies.size());

    for (size_t i = 0; i <= _strategies.size() - 3; i++){
        for (size_t j = i + 1; j <= _strategies.size() - 2; j++){
            for (size_t k = j + 1; k <= _strategies.size() - 1; k++){
                StrategiesTriplet competitors = extract_strategies_triplet({i, j, k});

                Score current_triple_score = competition(competitors);

                add_score(current_triple_score, score, {i, j, k});

                _viewer.view_round(score, current_triple_score, {i, j, k});
            }
        }
    }

    return score;
}

Game::Game(StrategiesVector& strategies,
           const StrategiesNames& strategies_names,
           const Matrix& matrix,
           unsigned int steps_count,
           GameMode mode,
           Storage& storage
           ):
        _viewer(strategies_names),
        _strategies(strategies),
        _matrix(matrix),
        _steps_count(steps_count),
        _mode(mode),
        _storage(storage) {}

void Game::start(){
    Score score;

    if (_mode != GameMode::TOURNAMENT){
        StrategiesTriplet competitors = extract_strategies_triplet({0, 1, 2});

        score = competition(competitors);
    }
    else{
        score = tournament();
    }

    GameViewer::view_final_score(score);

    _storage.dump_history();
}
