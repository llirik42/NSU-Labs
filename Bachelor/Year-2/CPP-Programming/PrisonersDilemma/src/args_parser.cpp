#include <regex>
#include <filesystem>
#include <functional>
#include "args_parser.h"

ParsingStatus validate_directory(const std::string& path){
    const std::filesystem::path file_system_path(path);

    try{
        return std::filesystem::is_directory(path) ? ParsingStatus::SUCCESS : ParsingStatus::INCORRECT_CONFIGS_PATH;
    }
    catch(...){
        return ParsingStatus::OPENING_CONFIGS_DIRECTORY_ERROR;
    }
}

inline bool points_to_end(const char* const pointer){
    return pointer[0] == '\0';
}

inline bool points_to_equal(const char* const pointer){
    return pointer[0] == '=';
}

void move_to_equal_and_step(const char** const pointer){
    while (!points_to_equal(*pointer)){
        (*pointer)++;
    }

    (*pointer)++;
}

std::string extract_string_arg_value(const char* arg){
    std::string result;

    move_to_equal_and_step(&arg);

    while (!points_to_end(arg)){
        result += *arg;
        arg++;
    }

    return result;
}

ParsingStatus validate_args(int arc, const char** argv){
    std::regex arg_regex(R"(--((configs=.+)|(help)|(steps=([1-9])([0-9])*)|(matrix=.+)|(mode=((fast)|(detailed)|(tournament)))|(strategies=(\[).+,.+,.+(,.+)*(\]))))");

    for (int i = 1; i < arc; i++){
        if (!std::regex_match(argv[i], arg_regex)){
            return ParsingStatus::INCORRECT_ARGS;
        }
    }

    return ParsingStatus::SUCCESS;
}

ParsingStatus validate_count_of_met_args(const ArgsParser::MetArgsMap& met_args){
    bool met_some_arg = false;
    for (const auto& [arg_name, met] : met_args){
        if (met){
            met_some_arg = true;
        }
    }

    return met_some_arg ? ParsingStatus::SUCCESS : ParsingStatus::TOO_FEW_ARG;
}

ParsingStatus validate_strategies(const ArgsParser& parser, const StrategiesDescription& description){
    for (size_t i = 0; i < parser._strategies_names.size(); i++){
        const auto current_strategy_name = parser._strategies_names[i];

        bool met_unknown_strategy = false;

        for (const auto& [strategy_name, strategy_description] : description){
            if (strategy_name == current_strategy_name){
                met_unknown_strategy = true;
                break;
            }
        }

        if (!met_unknown_strategy){
            return ParsingStatus::UNKNOWN_STRATEGIES;
        }

        for (unsigned int j = 0; j < i; j++){
            if (current_strategy_name == parser._strategies_names[j]){
                return ParsingStatus::REPEATED_STRATEGIES;
            }
        }
    }

    return ParsingStatus::SUCCESS;
}

bool is_help_only_arg(ArgsParser::MetArgsMap& met_args){
    if (met_args["--help"]){
        for (const auto& [arg_name, met]: met_args){
            if (arg_name != "--help" && met){
                return false;
            }
        }
    }

    return true;
}

std::string extract_arg_name(const char* arg){
    std::string result;

    const char* current_pointer = arg;
    while (!points_to_end(current_pointer) && !points_to_equal(current_pointer)){
        result += current_pointer[0];
        current_pointer++;
    }

    return result;
}

void extract_steps_count(const char* arg, ArgsParser& parser){
    parser._steps_count = 0;

    move_to_equal_and_step(&arg);

    while (!points_to_end(arg)){
        parser._steps_count = parser._steps_count * 10 + char_to_digit(*arg);
        arg++;
    }
}

void extract_matrix_file_path(const char* arg, ArgsParser& parser){
    parser._matrix_file_path = extract_string_arg_value(arg);
}

void extract_configs_path(const char* arg, ArgsParser& parser){
    parser._configs_path = extract_string_arg_value(arg);
}

void extract_game_mode(const char* arg, ArgsParser& parser){
    move_to_equal_and_step(&arg);

    // This arg was validated earlier, so it can be only "Detailed", "Fast" or "Tournament"
    switch (*arg){
        case 'd':
            parser._game_mode = GameMode::DETAILED;
            return;
        case 'f':
            parser._game_mode = GameMode::FAST;
            return;
        default:
            parser._game_mode = GameMode::TOURNAMENT;
    }
}

void extract_strategies_names(const char* arg, ArgsParser& parser){
    move_to_equal_and_step(&arg);

    arg++; // Because of '['

    while (!points_to_end(arg)){
        std::string current_strategy_name;

        while (*arg != ',' && *arg != ']'){
            current_strategy_name += *(arg++);
        }

        arg++; // Because of ','

        parser._strategies_names.push_back(current_strategy_name);
    }
}

void extract_args(int arc, const char** argv, ArgsParser::MetArgsMap& met_args, ArgsParser& parser){
    std::map<std::string, std::function<void(const char*, ArgsParser&)>> extracting_function({
        {"--strategies", extract_strategies_names},
        {"--mode", extract_game_mode},
        {"--steps", extract_steps_count},
        {"--matrix", extract_matrix_file_path},
        {"--configs", extract_configs_path}
    });

    for (int i = 1; i < arc; i++){
        auto current_arg_name = extract_arg_name(argv[i]);

        if (met_args[current_arg_name]){
            parser._parsing_status = ParsingStatus::REPEATED_ARGS;
            return;
        }

        met_args[current_arg_name] = true;

        if (current_arg_name != "--help"){
            extracting_function[current_arg_name](argv[i], parser);
        }
    }
}

void ArgsParser::parse(int arc, const char** argv, const StrategiesDescription& strategies_description){
    MetArgsMap met_args({
        {"--help", false},
        {"--strategies", false},
        {"--mode", false},
        {"--steps", false},
        {"--matrix", false},
        {"--configs", false}
    });

    extract_args(arc, argv, met_args, *this);

    ParsingStatus count_validation_status = validate_count_of_met_args(met_args);
    if (count_validation_status != ParsingStatus::SUCCESS){
        _parsing_status = count_validation_status;
        return;
    }

    if (met_args["--help"]){
        if (!is_help_only_arg(met_args)){
            _parsing_status = ParsingStatus::HELP_IS_NOT_ONLY_ARG;
        }

        _is_help = true;
        return;
    }

    if (!met_args["--strategies"]){
        _parsing_status = ParsingStatus::NO_STRATEGIES;
        return;
    }

    ParsingStatus strategies_validation_status = validate_strategies(*this, strategies_description);
    if (strategies_validation_status != ParsingStatus::SUCCESS){
        _parsing_status = strategies_validation_status;
        return;
    }

    if (!_configs_path.empty()){
        ParsingStatus validating_directory_status = validate_directory(_configs_path);

        if (validating_directory_status != ParsingStatus::SUCCESS){
            _parsing_status = validating_directory_status;
            return;
        }
    }

    if (_strategies_names.size() > 3 && !met_args["--mode"]){
        _game_mode = GameMode::TOURNAMENT;
    }

    if (met_args["--matrix"]){
        _is_default_matrix = false;
    }
}

ArgsParser::ArgsParser(int arc, const char** argv, const StrategiesDescription& strategies_description):
    _steps_count(1),
    _game_mode(GameMode::DETAILED),
    _parsing_status(ParsingStatus::SUCCESS),
    _is_default_matrix(true),
    _is_help(false){

    const ParsingStatus validation_status = validate_args(arc, argv);

    if (validation_status != ParsingStatus::SUCCESS){
        _parsing_status = validation_status;
        return;
    }

    parse(arc, argv, strategies_description);
}

ParsingStatus ArgsParser::get_parsing_status() const{
    return _parsing_status;
}

const StrategiesNames& ArgsParser::get_strategies_names() const{
    return _strategies_names;
}

unsigned int ArgsParser::get_steps_count() const{
    return _steps_count;
}

const std::string& ArgsParser::get_matrix_file_path() const{
    return _matrix_file_path;
}

const std::string& ArgsParser::get_configs_path() const{
    return _configs_path;
}

GameMode ArgsParser::get_game_mode() const{
    return _game_mode;
}

bool ArgsParser::is_help_mode() const{
    return _is_help;
}

bool ArgsParser::is_matrix_default() const{
    return _is_default_matrix;
}
