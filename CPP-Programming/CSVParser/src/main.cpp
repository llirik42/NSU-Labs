#include <iostream>

#include "csv_parsing.hpp"
#include "tuple_print.hpp"

int main(){
    try{
        const char* file_path = "../tmp.csv";
        char delimiter = ',';
        char escape_character = '\\';
        char newline_character = '\n';
        size_t skip_first_lines_count = 0;

        Parsing::Settings settings{delimiter, escape_character, newline_character};

        Parsing::CSVParser<std::string, int, float> parser(file_path, settings, skip_first_lines_count);

        for (auto it = parser.rbegin(); it != parser.rend(); it++){
            std::cout << *it;
        }
    }
    catch (const Exceptions::AbstractException& exception){
        std::cerr << exception.what() << '\n';
        return exception.code();
    }
    catch(...){
        std::cerr << "Something unpredicted happened\n";
        return Exceptions::UNEXPECTED_ERROR_CODE;
    }

    return Exceptions::SUCCESS;
}
