#pragma once

#include <fstream>
#include <string>
#include <map>

namespace Utils{
    size_t get_file_size(std::ifstream& file);

    bool is_file_empty(std::ifstream& file);

    std::string generate_random_wav_file_name();

    void copy_file(std::string_view from, std::string_view to);

    void rename_file(std::string_view old_name, std::string_view new_name);

    void remove_file(std::string_view file_path);

    size_t string_to_positive_number(std::string_view string);

    template<typename T1, typename T2>
    bool contains(const std::map<T1, T2>& map, const T1& value){
        return map.find(value)->first == value;
    }
}
