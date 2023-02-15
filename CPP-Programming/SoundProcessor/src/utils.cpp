#include <random>
#include "custom_exceptions.h"
#include "utils.h"

static const size_t COPYING_BUFFER_SIZE = 1024;

template<typename T>
char digit_to_char(T digit){
    return static_cast<char>(digit + '0');
}

size_t char_to_digit(char c){
    return static_cast<size_t>(c - '0');
}

char generate_random_letter(){
    static std::random_device device;
    static std::mt19937 engine(device());
    std::uniform_int_distribution<std::mt19937::result_type> distribution(0,51); // size of english alphabet * 2
    auto value = distribution(engine);
    auto result = value <= 25 ? value + 'a' : (value - 26) + 'A';
    return static_cast<char>(result);
}

char generate_random_digit(){
    static std::random_device device;
    static std::mt19937 engine(device());
    std::uniform_int_distribution<std::mt19937::result_type> distribution(0,9); // 0, 1, ..., 9
    return digit_to_char(distribution(engine));
}

char generate_random_alphanumeric(){
    static std::random_device device;
    static std::mt19937 engine(device());
    std::uniform_int_distribution<std::mt19937::result_type> distribution(0,1);
    auto value = distribution(engine);
    return value ? generate_random_letter() : generate_random_digit();
}

size_t Utils::get_file_size(std::ifstream& file){
    auto start_pos = file.tellg();

    file.seekg(0, std::fstream::end);

    auto result = static_cast<size_t>(file.tellg());

    file.seekg(start_pos);

    return result;
}

bool Utils::is_file_empty(std::ifstream& file){
    return !get_file_size(file);
}

std::string Utils::generate_random_wav_file_name(){
    static const size_t file_name_length = 128;

    std::string result;

    for (unsigned int i = 0; i < file_name_length; i++){
        result.append(1, generate_random_alphanumeric());
    }

    result.append(".wav");

    return result;
}

void Utils::copy_file(std::string_view from, std::string_view to){
    std::ofstream out(to.data(), std::fstream::binary);
    std::ifstream in(from.data(), std::fstream::binary);

    if (!out.is_open() || !in.is_open()){
        throw Exceptions::IOError();
    }

    char buffer[COPYING_BUFFER_SIZE];

    while (!in.eof()){
        in.read(buffer, COPYING_BUFFER_SIZE);

        out.write(buffer, in.gcount());
    }
}

void Utils::rename_file(std::string_view old_name, std::string_view new_name){
    if(std::rename(old_name.data(), new_name.data())){
        throw Exceptions::IOError();
    }
}

void Utils::remove_file(std::string_view file_path){
    try{
        std::remove(file_path.data());
    }
    catch(...){
        throw Exceptions::IOError();
    }
}

size_t Utils::string_to_positive_number(std::string_view string){
    size_t result = 0;

    if (string[0] == '0' && string.size() != 1){
        throw std::runtime_error("Conversion error");
    }


    for (const auto& c : string){
        if (!std::isdigit(c)){
            throw std::runtime_error("Conversion error");
        }

        result = result * 10 + char_to_digit(c);
    }

    return result;
}
