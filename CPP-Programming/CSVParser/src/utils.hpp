#pragma once

#include <cstddef>
#include <fstream>
#include <string>

namespace Utils{
    void trim_back_spaces(std::string& string);

    std::streamoff count_line_pos(std::ifstream& ifstream, size_t line_number, char newline_character);

    std::streamoff get_last_position_in_file(std::ifstream& ifstream);

    void read_line_backwards(std::ifstream& ifstream, std::string& string, char delimiter);

    bool is_empty_line(const std::string& string);

    bool is_next_line_empty(std::ifstream& ifstream);

    void read_empty_lines_backwards(std::ifstream& ifstream);
}
