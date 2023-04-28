#pragma once

#include <vector>
#include <array>
#include <string>
#include <map>
#include "utils.h"
#include "strategies/abstract_strategy.h"

class AbstractStorage{
public:
    [[nodiscard]] virtual bool is_current_game_history_empty() const = 0;
    [[nodiscard]] virtual Round get_previous_round_info() const; // In current game
    [[nodiscard]] virtual History get_previous_games_history() const;

    virtual void append_round(const Round& round);

    virtual ~AbstractStorage()=default;
protected:
    AbstractStorage()=default;
private:
};

class Storage:public AbstractStorage{
public:
    Storage()=default;
    explicit Storage(const std::string& configs_path);

    [[nodiscard]] bool is_current_game_history_empty() const override;
    [[nodiscard]] Round get_previous_round_info() const override;
    [[nodiscard]] History get_previous_games_history() const override;

    void append_round(const Round& round) override;

    void dump_history();
private:
    std::string _configs_path;
    History _current_game_history;
    History _previous_games_history;
};
