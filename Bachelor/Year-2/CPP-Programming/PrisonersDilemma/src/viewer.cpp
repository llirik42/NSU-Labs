#include <iostream>
#include <limits>
#include "viewer.h"

struct ArgsInfo{
    std::string description;
    bool is_necessary;
};

inline const unsigned int WIDTH2 = 20;
inline const unsigned int WIDTH_FOR_LIST = 40;
inline const unsigned int LIST_INDENT = 10;

using ListWithDescription = std::map<std::string, std::string>;

template<typename Container, typename Element>
void view_container(const std::string& title, const Container& container, bool unary_plus){
    if (unary_plus){
        std::cout.setf(std::ostream::showpos);
    }

    std::cout.setf(std::ostream::right);
    std::cout.fill(' ');

    if (!title.empty()){
        std::cout << title << ":\n";
    }

    for (const Element& i : container){
        std::cout.width(WIDTH2);
        std::cout << i;
    }
    std::cout << "\n";

    std::cout.unsetf(std::ostream::right);
    std::cout.unsetf(std::ostream::showpos);
}

void view_list(const std::string& list_title, const ListWithDescription& list, bool args){
    std::cout << list_title << ":\n";
    for (const auto& [element, description] : list){
        for (unsigned int i = 0; i < LIST_INDENT; i++){
            std::cout << ' ';
        }
        if (args){
            std::cout << "--";
        }

        std::cout.width(WIDTH_FOR_LIST + 2 * (!args));
        std::cout.setf(std::ostream::left);
        std::cout << element << description << '\n';
        std::cout.unsetf(std::ostream::left);
    }
}

GameViewer::GameViewer(): _players_count(0), _rounds_counter(0) {}

GameViewer::GameViewer(const StrategiesNames& strategies_names): _players_count(strategies_names.size()), _rounds_counter(0){
    for (const auto& name : strategies_names){
        std::cout.width(WIDTH2);

        std::cout << name;
    }

    std::cout << '\n';
}

void GameViewer::view_round(const Score& current_score, const Score& delta_score, const StrategiesIndexes& indexes){
    std::cout << "Round " << (1 + _rounds_counter++) << ":\n";

    std::cout.setf(std::ostream::showpos);
    std::cout.setf(std::ostream::right);
    std::cout.fill(' ');

    const unsigned int index_of_failure = std::numeric_limits<unsigned int>::max();

    for (size_t i = 0; i < _players_count; i++){
        std::cout.width(WIDTH2);

        unsigned int tmp_index = index_of_failure;
        for (size_t j = 0; j < indexes.size(); j++){
            if (indexes[j] == i){
                tmp_index = j;
            }
        }

        if (tmp_index != index_of_failure){
            std::cout << delta_score[tmp_index];
        }
        else{
            std::cout << " ";
        }
    }
    std::cout << "\n";

    std::cout.unsetf(std::ostream::right);
    std::cout.unsetf(std::ostream::showpos);

    view_container<const Score&, int>("", current_score, false);
    std::cout << '\n';
}

void GameViewer::view_incorrect_matrix(){
    std::cerr << "Incorrect matrix\n";
}

void GameViewer::view_incorrect_command(){
    std::cerr << "Incorrect command!\n\n";
}

void GameViewer::view_final_score(const Score& final_score){
    view_container<const Score&, int>("Final score", final_score, false);
}

void GameViewer::view_help_command(const StrategiesDescription& strategies_description){
    std::cout << "The program is the simulator of Prisoner's dilemma for 3 players. It arranges classical rounds "
                 "with 3 players and tournament, where all kinds of threes are being competed.\n\n";

    std::map<std::string, ArgsInfo> args_info({
        {"help", {"Help", false}},
        {"matrix=MATRIX_PATH", {"Path to file of matrix", false}},
        {"steps=STEPS", {"Number of steps in game (in a round if mode=tournament)", false}},
        {"mode=detailed|fast|tournament", {R"(Game of mode. "detailed" can be interrupted by the "quit")", false}},
        {"strategies=[S1,S2,S3,...]", {"List of strategies in the game (at least 3). Strategies must go with no spaces and repeats", true}},
        {"configs=CONFIGS_PATH", {"Directory with files of simulator" , false}},
        });

    ListWithDescription args_description;

    std::cout << "Usage PrisonersDilemma ";
    for (const auto& [arg, info] : args_info){
        bool is_arg_necessary = info.is_necessary;

        if (!is_arg_necessary){
            std::cout << '[';
        }

        std::cout << "--" << arg;

        if (!is_arg_necessary){
            std::cout << "] ";
        }

        args_description[arg] = info.description;
    }
    std::cout << "\n\n";

    ListWithDescription modes_description({
        {"fast", "The program calculates the specified number of moves and outputs the result"},
        {"detailed", "At each step, the program waits for a keystroke and after it prints data for this round and takes one step"},
        {"tournament", "The program iterates through all possible triples and identifies the winner by the sum of points"}
    });

    view_list("Options", args_description, true);
    view_list("Strategies", strategies_description, false);
    view_list("Modes", modes_description, false);
}

void GameViewer::view_round(const Score& current_score, const Choices& choices, const Score& delta_score){
    view_container<const Choices, Step>("", choices, false);
    view_container<const Score, int>("", delta_score, true);
    view_container<const Score, int>("", current_score, false);
}

void GameViewer::view_parsing_error(ParsingStatus status){
    static std::map<ParsingStatus, std::string> status_to_message({
        {ParsingStatus::TOO_FEW_ARG,                            "Too few args"},
        {ParsingStatus::HELP_IS_NOT_ONLY_ARG,                   "--help is not the only arg"},
        {ParsingStatus::TOO_MANY_STRATEGIES_FOR_NON_TOURNAMENT, "Too many strategies for non-tournament mode"},
        {ParsingStatus::INCORRECT_ARGS,                         "Incorrect args"},
        {ParsingStatus::REPEATED_ARGS,                          "Some args are repeated"},
        {ParsingStatus::UNKNOWN_STRATEGIES,                     "Unknown strategies"},
        {ParsingStatus::NO_STRATEGIES,                          "At least 3 strategies must be chosen"},
        {ParsingStatus::INCORRECT_CONFIGS_PATH,                 "Incorrect path of configs"},
        {ParsingStatus::OPENING_CONFIGS_DIRECTORY_ERROR,        "Error occurred while opening configs directory"},
        {ParsingStatus::REPEATED_STRATEGIES, "Some strategies are repeated"}
        });

    std::cerr << status_to_message[status] << ". See --help";
}

void GameViewer::view_exception(const std::exception& exception){
    std::cerr << exception.what();
}
