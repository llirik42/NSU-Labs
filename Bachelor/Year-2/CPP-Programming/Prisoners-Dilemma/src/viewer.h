#pragma once

#include "utils.h"
#include "strategies_factory.h"
#include "args_parser.h"

class GameViewer{
public:
    GameViewer();
    explicit GameViewer(const StrategiesNames& strategies_names);

    void view_round(const Score& current_score, const Score& delta_score, const StrategiesIndexes& indexes);

    static void view_incorrect_matrix();
    static void view_incorrect_command() ;
    static void view_parsing_error(ParsingStatus status);
    static void view_final_score(const Score& final_score);
    static void view_help_command(const StrategiesDescription& strategies_description);
    static void view_round(const Score& current_score, const Choices& choices, const Score& delta_score);
    static void view_exception(const std::exception& exception);
private:
    size_t _players_count;
    unsigned int _rounds_counter;
};
