#pragma once

#include <array>
#include <vector>
#include <map>
#include <string>

template<typename type>
using Triplet = std::array<type, 3>;

using Step = char;

using Choices = std::vector<Step>;

using Round = std::map<std::string, Step>; // What strategy and what did in the round

using Score = std::vector<int>;

using StrategiesIndexes = Triplet<size_t>;

using StrategiesDescription = std::map<std::string, std::string>;

using StrategiesNames = std::vector<std::string>;

using History = std::vector<Round>;

inline const Step COOPERATION_STEP = 'C';
inline const Step DEFECTION_STEP = 'D';

inline int char_to_digit(char c){
    return c - '0';
}

enum class GameMode{
    FAST,
    DETAILED,
    TOURNAMENT,
};
