#include <algorithm>
#include <string>

#include "utils.hpp"

void Utils::trim_back_spaces(std::string& string){
    static const char SPACE = ' ';
    while (string.back() == SPACE){
        string.erase(string.size() - 1, 1);
    }
}

std::streamoff Utils::count_line_pos(std::ifstream& ifstream, size_t line_number, char newline_character){
    std::string line;
    for (unsigned int i = 0; i < line_number; i++){
        std::getline(ifstream, line, newline_character);
    }

    auto line_pos = ifstream.tellg();

    return line_pos;
}

std::streamoff Utils::get_last_position_in_file(std::ifstream& ifstream){
    ifstream.seekg(-1, std::fstream::end);

    char c;
    ifstream.get(c);
    ifstream.seekg(-1, std::fstream::end);

    if (std::isspace(c)){
        read_empty_lines_backwards(ifstream);
    }

    return ifstream.tellg();
}

void Utils::read_line_backwards(std::ifstream& ifstream, std::string& string, char delimiter){
    string.clear();

    char c;
    ifstream.get(c);

    while (c != delimiter && !ifstream.fail()){
        string.push_back(c);
        ifstream.seekg(-2, std::ios::cur);
        ifstream.get(c);
    }

    std::reverse(string.begin(), string.end());

    // seek before delimiter
    ifstream.seekg(-2, std::fstream::cur);
}

bool Utils::is_empty_line(const std::string& string){
    return std::all_of(string.begin(), string.end(), [](const auto& c)
    {
        return std::isspace(c);
    });
}

bool Utils::is_next_line_empty(std::ifstream& ifstream){
    auto start_position = ifstream.tellg();

    std::string next_line;

    std::getline(ifstream, next_line);

    ifstream.seekg(start_position);

    return is_empty_line(next_line);
}

void Utils::read_empty_lines_backwards(std::ifstream& ifstream){
    char c;
    ifstream.get(c);

    while (std::isspace(c)){
        ifstream.seekg(-2, std::ios::cur);
        ifstream.get(c);
    }
}
