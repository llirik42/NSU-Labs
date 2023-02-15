#include <fstream>
#include <regex>
#include "matrix.h"

inline const unsigned int ELEMENTS_IN_ROW_COUNT = 3;

using RowsMetTable = std::map<Choices, unsigned int>;

using namespace AllChoices;

Choices Matrix::match_row(const std::string& line){
    std::string line_copy = line;

    static const std::string pattern = {'[', C, D, ']'};

    // regex for matching letters in row of matrix
    static const std::regex letters_regex(pattern);

    // regex for matching numbers in row of matrix
    static const std::regex numbers_regex(R"(0|(-?[1-9]\d*))");

    Choices current_row_code; // code of current row (like {C, C, C}, {C, D, C} or anything else)

    std::smatch smatch; // matches

    // matching of row-code
    for (unsigned int i = 0; i < ELEMENTS_IN_ROW_COUNT; i++){
        regex_search(line_copy, smatch, letters_regex);
        current_row_code.push_back(smatch[0].str()[0]);
        line_copy = smatch.suffix();
    }

    _content[current_row_code].resize(ELEMENTS_IN_ROW_COUNT);

    // matching of numbers
    for (unsigned int i = 0; i < ELEMENTS_IN_ROW_COUNT; i++){
        regex_search(line_copy, smatch, numbers_regex);
        _content[current_row_code][i] = std::stoi(smatch[0]);
        line_copy = smatch.suffix();
    }

    return current_row_code;
}

bool check_rows_met_table(const RowsMetTable& table){
    return std::all_of(table.begin(), table.end(), [](const auto& pair)
    {
        return pair.second == 1;
    });
}

bool Matrix::input(std::ifstream& ifstream){
    /*
     * If file is incorrect - false, else - true
    */

    // empty line (maybe with comments)
    static const std::regex empty_line_regex(R"((//.*)|(\s*))");

    // line with row of matrix
    static const std::regex row_regex(R"(\s*([CD]\s+){3}((0|(-?[1-9]\d*))\s+){2}(0|(-?[1-9]\d*))(\s*|\s+//.*))");

    std::map<Choices, unsigned int> rows_met_table({
        {CCC, 0},
        {CCD, 0},
        {CDC, 0},
        {CDD, 0},
        {DCC, 0},
        {DCD, 0},
        {DDC, 0},
        {DDD, 0},
        });

    while(!ifstream.eof()){
        std::string current_line;
        std::getline(ifstream, current_line);

        if (std::regex_match(current_line.data(), empty_line_regex)){
            continue;
        }

        if (std::regex_match(current_line.data(), row_regex)){
            Choices current_row_code = match_row(current_line);

            rows_met_table[current_row_code]++;

            continue;
        }
        return false;
    }

    return check_rows_met_table(rows_met_table);
}

bool Matrix::is_symmetric() const{
    /* Checks matrix for symmetry
     * For example, in row "CCC" all 3 values must be equal
     * In rows "CCD", "DCC", "CDC" values of single "D" must be equal and ...
    */

    // values of "CCC" row
    const int c01 = get_element(CCC, 0);
    const int c02 = get_element(CCC, 1);
    const int c03 = get_element(CCC, 2);
    const bool condition_1 = c01 == c02 && c01 == c03;

    // values of C`s where 2 C's and 1 D
    const int c11 = get_element(CCD, 0);
    const int c12 = get_element(CCD, 1);
    const int c13 = get_element(CDC, 0);
    const int c14 = get_element(CDC, 2);
    const int c15 = get_element(DCC, 1);
    const int c16 = get_element(DCC, 2);
    const bool condition_2 = c11 == c12 && c11 == c13 && c11 == c14 && c11 == c15 && c11 == c16;

    // values of C`s where 1 C and 2 D`s
    const int c21 = get_element(CDD, 0);
    const int c22 = get_element(DCD, 1);
    const int c23 = get_element(DDC, 2);
    const bool condition_3 = c21 == c22 && c21 == c23;

    // values of D`s where 1 D and 2 C`s
    const int d01 = get_element(CCD, 2);
    const int d02 = get_element(CDC, 1);
    const int d03 = get_element(DCC, 0);
    const bool condition_4 = d01 == d02 && d01 == d03;

    // values of D`s where 2 D's and 1 C
    const int d11 = get_element(CDD, 1);
    const int d12 = get_element(CDD, 2);
    const int d13 = get_element(DCD, 0);
    const int d14 = get_element(DCD, 2);
    const int d15 = get_element(DDC, 0);
    const int d16 = get_element(DDC, 1);
    const bool condition_5 = d11 == d12 && d11 == d13 && d11 == d14 && d11 == d15 && d11 == d16;

    // values of "DDD" row
    const int d21 = get_element(DDD, 0);
    const int d22 = get_element(DDD, 1);
    const int d23 = get_element(DDD, 2);
    const bool condition_6 = d21 == d22 && d21 == d23;

    return condition_1 && condition_2 && condition_3 && condition_4 && condition_5 && condition_6;
}

Matrix::Matrix(): _has_error(false){
    _content = {
            {CCC, {7, 7, 7}},
            {CCD, {3, 3, 9}},
            {CDC, {3, 9, 3}},
            {DCC, {9, 3, 3}},
            {CDD, {0, 5, 5}},
            {DCD, {5, 0, 5}},
            {DDC, {5, 5, 0}},
            {DDD, {1, 1, 1}},
    };
}
Matrix::Matrix(const std::string& path){
    try{
        std::ifstream ifstream(path);

        // path is incorrect
        if (!ifstream.is_open()){
            _has_error = true;
            return;
        }

        _has_error = !input(ifstream);
    }
    catch(...){
        throw std::runtime_error("Error while opening matrix file");
    }
}

bool Matrix::has_error() const{
    return _has_error;
}

bool Matrix::is_consistent() const{
    if (!is_symmetric()){
        return false;
    }

    /*
     * C C C c0 c0 c0
     * C C D c1 c1 d0
     * C D C d1 d0 d1
     * C D D c2 d1 d1
     * D C C d0 c1 c1
     * D C D d1 d2 d1
     * D D C d1 d1 c2
     * D D D d2 d2 d2
     *
     * Four inequalities must be checked:
     * 1) 3 * c0 > 2 * c1 + d0
     * 2) 3 * c0 + 2 * d1 + c2
     * 3) 3 * c0 > 3 * d2
     * 4) d0 > c0 > d1 > c1 > d2 > c2
     *
     * Last one sets relations between betrayal and cooperation.
     * First three ones tell that total score of general cooperation is greater than score of any
     * other choices, but 4) implies 2) and 3), so we have to check only 1) and 4).
     * We assume that matrix is symmetric in this step.
     */

    const int c0 = get_element(CCC, 0);
    const int c1 = get_element(CCD, 0);
    const int c2 = get_element(CDD, 0);

    const int d0 = get_element(CCD, 2);
    const int d1 = get_element(DCD, 0);
    const int d2 = get_element(DDD, 0);

    // This tells that cooperation is more beneficial in a long game
    const bool inequality_1 = (3 * c0 > 2 * c1 + d0);

    // This sets relations between betrayal and cooperation
    const bool inequality_2 = (d0 > c0) && (c0 > d1) && (d1 > c1) && (c1 > d2) && (d2 > c2);

    return inequality_1 && inequality_2;
}

const Row& Matrix::operator[](const Choices& choices) const{
    return _content.at(choices);
}

int Matrix::get_element(const Choices& row_code, unsigned int index_in_row) const{
    return _content.at(row_code)[index_in_row];
}
