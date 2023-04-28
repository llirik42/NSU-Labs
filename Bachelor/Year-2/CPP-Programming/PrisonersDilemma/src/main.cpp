#include "strategies_factory.h"
#include "args_parser.h"
#include "game.h"
#include "storage.h"
#include "random_init.h"

int main(int arc, const char** argv){
    init_random();

    StrategiesFactory factory = StrategiesFactory();

    StrategiesDescription strategies_description = factory.get_strategies_description();

    try{
        ArgsParser args_parser(arc, argv, strategies_description);

        ParsingStatus status = args_parser.get_parsing_status();

        if (status != ParsingStatus::SUCCESS){
            GameViewer::view_parsing_error(status);
            return 1;
        }

        if (args_parser.is_help_mode()){
            GameViewer::view_help_command(strategies_description);
            return 0;
        }

        Matrix matrix = args_parser.is_matrix_default() ? Matrix() : Matrix(args_parser.get_matrix_file_path());
        if (matrix.has_error()){
            GameViewer::view_incorrect_matrix();
            return 1;
        }

        Storage storage = Storage(args_parser.get_configs_path());

        History previous_games_history = storage.get_previous_games_history();

        const bool is_previous_games_history_not_empty = !previous_games_history.empty();

        StrategiesNames strategies_names = args_parser.get_strategies_names();
        StrategiesVector strategies;
        for (const auto& name : strategies_names){
            Strategy new_strategy = factory.create_strategy(name);

            if (is_previous_games_history_not_empty){
                new_strategy->apply_previous_games_experience(previous_games_history);
            }

            strategies.push_back(new_strategy);
        }

        Game game(strategies, strategies_names, matrix, args_parser.get_steps_count(),
                  args_parser.get_game_mode(), storage);

        game.start();
    }
    catch(const std::exception& exception){
        GameViewer::view_exception(exception);
    }

    return 0;
}
