#include <tuple>

#include "gtest/gtest.h"

#include "../src/csv_parsing.hpp"

TEST(CSVParserTests, test_incorrect_path){
    EXPECT_THROW(Parsing::CSVParser<> parser("incorrect path", {}, 0), Exceptions::OpeningCSVError);
}

TEST(CSVParserTests, test_empty){
    EXPECT_THROW(Parsing::CSVParser<>("../tests/csv_files/0.csv", {}, 1),
                 Exceptions::IncorrectSkipFirstLinesCountError);

    EXPECT_THROW(Parsing::CSVParser<>("../tests/csv_files/0.csv", {}, 2),
                 Exceptions::IncorrectSkipFirstLinesCountError);

    Parsing::CSVParser<int, int> parser("../tests/csv_files/0.csv", {}, 0);

    EXPECT_THROW(parser.begin(), Exceptions::IncorrectCSVContentError);

    EXPECT_THROW(*parser.end(), Exceptions::ValueOfEndIteratorError);
    EXPECT_THROW(parser.end()++, Exceptions::IteratorOutOfRangeError);
}

TEST(CSVParserTests, test_incorrect_skip_first_lines_count_2){
    EXPECT_THROW(Parsing::CSVParser<> parser("../tests/csv_files/14.csv", {}, 2),
                 Exceptions::IncorrectSkipFirstLinesCountError);
}

TEST(CSVParserTests, test_incorrect_skip_first_lines_count_3){
    EXPECT_THROW(Parsing::CSVParser<> parser("../tests/csv_files/16.csv", {}, 10),
                 Exceptions::IncorrectSkipFirstLinesCountError);
}

TEST(CSVParserTests, test_correct_int_1){
    Parsing::CSVParser<int> parser("../tests/csv_files/1.csv", {}, 0);

    std::vector<std::tuple<int>> expected_tuples = {
            {0}
    };

    auto it_expected = expected_tuples.begin();
    auto it = parser.begin();

    for (; it != parser.end(); it_expected++, it++){
        EXPECT_EQ(*it_expected, *it);
    }

    EXPECT_THROW(*it, Exceptions::ValueOfEndIteratorError);
    EXPECT_THROW(it++, Exceptions::IteratorOutOfRangeError);

    EXPECT_THROW(*parser.end(), Exceptions::ValueOfEndIteratorError);
    EXPECT_THROW(parser.end()++, Exceptions::IteratorOutOfRangeError);
}

TEST(CSVParserTests, test_correct_int_2){
    const Parsing::Settings settings = {',', '\\', '\n'};
    const size_t skip_first_lines_count = 0;

    Parsing::CSVParser<int> parser("../tests/csv_files/2.csv", settings, skip_first_lines_count);

    std::vector<std::tuple<int>> expected_tuples = {
            {1},
            {2},
            {3},
            {4}
    };

    auto it_expected = expected_tuples.begin();
    auto it = parser.begin();

    for (; it != parser.end(); it_expected++, it++){
        EXPECT_EQ(*it_expected, *it);
    }

    EXPECT_THROW(*it, Exceptions::ValueOfEndIteratorError);
    EXPECT_THROW(it++, Exceptions::IteratorOutOfRangeError);

    EXPECT_THROW(*parser.end(), Exceptions::ValueOfEndIteratorError);
    EXPECT_THROW(parser.end()++, Exceptions::IteratorOutOfRangeError);
}

TEST(CSVParserTests, test_correct_int_3){
    const Parsing::Settings settings = {',', '\\', '\n'};
    const size_t skip_first_lines_count = 0;

    Parsing::CSVParser<int> parser("../tests/csv_files/3.csv", settings, skip_first_lines_count);

    std::vector<std::tuple<int>> expected_tuples = {
            {-1},
            {1},
            {0},
            {2},
            {3},
            {10},
            {42}
    };

    auto it_expected = expected_tuples.begin();
    auto it = parser.begin();

    for (; it != parser.end(); it_expected++, it++){
        EXPECT_EQ(*it_expected, *it);
    }

    EXPECT_THROW(*it, Exceptions::ValueOfEndIteratorError);
    EXPECT_THROW(it++, Exceptions::IteratorOutOfRangeError);

    EXPECT_THROW(*parser.end(), Exceptions::ValueOfEndIteratorError);
    EXPECT_THROW(parser.end()++, Exceptions::IteratorOutOfRangeError);
}

TEST(CSVParserTests, test_correct_int_skip){
    const Parsing::Settings settings = {',', '\\', '\n'};
    const size_t skip_first_lines_count = 2;

    Parsing::CSVParser<int> parser("../tests/csv_files/8.csv", settings, skip_first_lines_count);

    std::vector<std::tuple<int>> expected_tuples = {
            {0},
            {2},
            {3},
            {10},
            {42}
    };

    auto it_expected = expected_tuples.begin();
    auto it = parser.begin();

    for (; it != parser.end(); it_expected++, it++){
        EXPECT_EQ(*it_expected, *it);
    }

    EXPECT_THROW(*it, Exceptions::ValueOfEndIteratorError);
    EXPECT_THROW(it++, Exceptions::IteratorOutOfRangeError);

    EXPECT_THROW(*parser.end(), Exceptions::ValueOfEndIteratorError);
    EXPECT_THROW(parser.end()++, Exceptions::IteratorOutOfRangeError);
}

TEST(CSVParserTests, test_correct_int_int_1){
    const Parsing::Settings settings = {',', '\\', '\n'};
    const size_t skip_first_lines_count = 0;

    Parsing::CSVParser<int, int> parser("../tests/csv_files/5.csv", settings, skip_first_lines_count);

    std::vector<std::tuple<int, int>> expected_tuples = {
            {1, 2}
    };

    auto it_expected = expected_tuples.begin();
    auto it = parser.begin();

    for (; it != parser.end(); it_expected++, it++){
        EXPECT_EQ(*it_expected, *it);
    }

    EXPECT_THROW(*it, Exceptions::ValueOfEndIteratorError);
    EXPECT_THROW(it++, Exceptions::IteratorOutOfRangeError);

    EXPECT_THROW(*parser.end(), Exceptions::ValueOfEndIteratorError);
    EXPECT_THROW(parser.end()++, Exceptions::IteratorOutOfRangeError);
}

TEST(CSVParserTests, test_correct_int_int_2){
    const Parsing::Settings settings = {',', '\\', '\n'};
    const size_t skip_first_lines_count = 0;

    Parsing::CSVParser<int, int> parser("../tests/csv_files/6.csv", settings, skip_first_lines_count);

    std::vector<std::tuple<int, int>> expected_tuples = {
            {1, 2},
            {3, 4},
            {5, 6},
            {7, 8}
    };

    auto it_expected = expected_tuples.begin();
    auto it = parser.begin();

    for (; it != parser.end(); it_expected++, it++){
        EXPECT_EQ(*it_expected, *it);
    }

    EXPECT_THROW(*it, Exceptions::ValueOfEndIteratorError);
    EXPECT_THROW(it++, Exceptions::IteratorOutOfRangeError);

    EXPECT_THROW(*parser.end(), Exceptions::ValueOfEndIteratorError);
    EXPECT_THROW(parser.end()++, Exceptions::IteratorOutOfRangeError);
}

TEST(CSVParserTests, test_correct_int_int_skip){
    const Parsing::Settings settings = {',', '\\', '\n'};
    const size_t skip_first_lines_count = 2;

    Parsing::CSVParser<int, int> parser("../tests/csv_files/7.csv", settings, skip_first_lines_count);

    std::vector<std::tuple<int, int>> expected_tuples = {
            {5, 6},
            {7, 8}
    };

    auto it_expected = expected_tuples.begin();
    auto it = parser.begin();

    for (; it != parser.end(); it_expected++, it++){
        EXPECT_EQ(*it_expected, *it);
    }

    EXPECT_THROW(*it, Exceptions::ValueOfEndIteratorError);
    EXPECT_THROW(it++, Exceptions::IteratorOutOfRangeError);

    EXPECT_THROW(*parser.end(), Exceptions::ValueOfEndIteratorError);
    EXPECT_THROW(parser.end()++, Exceptions::IteratorOutOfRangeError);
}

TEST(CSVParserTests, test_correct_int_float_string_char){
    const Parsing::Settings settings = {',', '\\', '\n'};
    const size_t skip_first_lines_count = 0;

    Parsing::CSVParser<int, float, std::string, char> parser("../tests/csv_files/4.csv", settings,
                                                             skip_first_lines_count);

    std::vector<std::tuple<int, float, std::string, char>> expected_tuples = {
            {123, 0.5, "text", 'c'},
            {109, -0.5, "other", 'q'}
    };

    auto it_expected = expected_tuples.begin();
    auto it = parser.begin();

    for (; it != parser.end(); it_expected++, it++){
        EXPECT_EQ(*it_expected, *it);
    }

    EXPECT_THROW(*it, Exceptions::ValueOfEndIteratorError);
    EXPECT_THROW(it++, Exceptions::IteratorOutOfRangeError);

    EXPECT_THROW(*parser.end(), Exceptions::ValueOfEndIteratorError);
    EXPECT_THROW(parser.end()++, Exceptions::IteratorOutOfRangeError);
}

TEST(CSVParserTests, test_correct_strings_with_delimiter){
    const Parsing::Settings settings = {',', '\\', '\n'};
    const size_t skip_first_lines_count = 0;

    Parsing::CSVParser<std::string, std::string, std::string> parser("../tests/csv_files/10.csv",
                                                                     settings, skip_first_lines_count);

    std::vector<std::tuple<std::string, std::string, std::string>> expected_tuples = {
            {"Hello, world!", "Well, I was there", "I love C,C++,C#"}
    };

    auto it_expected = expected_tuples.begin();
    auto it = parser.begin();

    for (; it != parser.end(); it_expected++, it++){
        EXPECT_EQ(*it_expected, *it);
    }

    EXPECT_THROW(*it, Exceptions::ValueOfEndIteratorError);
    EXPECT_THROW(it++, Exceptions::IteratorOutOfRangeError);

    EXPECT_THROW(*parser.end(), Exceptions::ValueOfEndIteratorError);
    EXPECT_THROW(parser.end()++, Exceptions::IteratorOutOfRangeError);
}

TEST(CSVParserTests, test_correct_strings_with_delimiter_and_escape_character){
    const Parsing::Settings settings = {',', '\\', '\n'};
    const size_t skip_first_lines_count = 0;

    Parsing::CSVParser<std::string, std::string, std::string> parser("../tests/csv_files/17.csv",
                                                                     settings, skip_first_lines_count);

    std::vector<std::tuple<std::string, std::string, std::string>> expected_tuples = {
            {R"(Hello\\, \\world!)", R"(Well, \I was there)", R"(I love C,C++,C#)"}
    };

    auto it_expected = expected_tuples.begin();
    auto it = parser.begin();

    for (; it != parser.end(); it_expected++, it++){
        EXPECT_EQ(*it_expected, *it);
    }

    EXPECT_THROW(*it, Exceptions::ValueOfEndIteratorError);
    EXPECT_THROW(it++, Exceptions::IteratorOutOfRangeError);

    EXPECT_THROW(*parser.end(), Exceptions::ValueOfEndIteratorError);
    EXPECT_THROW(parser.end()++, Exceptions::IteratorOutOfRangeError);
}

TEST(CSVParserTests, test_correct_mixed_with_unusual_settings){
    const Parsing::Settings settings = {'-', '"', '#'};
    const size_t skip_first_lines_count = 0;

    Parsing::CSVParser<std::string, float, int> parser("../tests/csv_files/9.csv", settings,
                                                       skip_first_lines_count);

    std::vector<std::tuple<std::string, float, int>> expected_tuples = {
            {"Hello, world!", -0.5, -13},
            {"Wow - it works!", -1.0, -2}
    };

    auto it_expected = expected_tuples.begin();
    auto it = parser.begin();

    for (; it != parser.end(); it_expected++, it++){
        EXPECT_EQ(*it_expected, *it);
    }

    EXPECT_THROW(*it, Exceptions::ValueOfEndIteratorError);
    EXPECT_THROW(it++, Exceptions::IteratorOutOfRangeError);

    EXPECT_THROW(*parser.end(), Exceptions::ValueOfEndIteratorError);
    EXPECT_THROW(parser.end()++, Exceptions::IteratorOutOfRangeError);
}

TEST(CSVParserTests, test_incorrect_integer){
    const Parsing::Settings settings = {',', '\\', '\n'};
    const size_t skip_first_lines_count = 0;

    Parsing::CSVParser<int, int, int> parser("../tests/csv_files/13.csv", settings, skip_first_lines_count);

    std::vector<std::tuple<int, int, int>> expected_tuples = {
            {12, 23, 15},
            {}
    };

    auto it_expected = expected_tuples.begin();
    auto it = parser.begin();

    EXPECT_EQ(*it_expected, *it);

    EXPECT_THROW(it++, Exceptions::IncorrectCSVContentError);

    EXPECT_THROW(*parser.end(), Exceptions::ValueOfEndIteratorError);
    EXPECT_THROW(parser.end()++, Exceptions::IteratorOutOfRangeError);
}

TEST(CSVParserTests, test_incorrect_float){
    const Parsing::Settings settings = {',', '\\', '\n'};
    const size_t skip_first_lines_count = 0;

    Parsing::CSVParser<float, float, float> parser("../tests/csv_files/12.csv", settings,
                                                   skip_first_lines_count);

    std::vector<std::tuple<float, float, float>> expected_tuples = {
            {12.1f, 23.2f, 15.3f},
            {}
    };

    auto it_expected = expected_tuples.begin();
    auto it = parser.begin();

    EXPECT_EQ(*it_expected, *it);

    EXPECT_THROW(it++, Exceptions::IncorrectCSVContentError);

    EXPECT_THROW(*parser.end(), Exceptions::ValueOfEndIteratorError);
    EXPECT_THROW(parser.end()++, Exceptions::IteratorOutOfRangeError);
}

TEST(CSVParserTests, test_incorrect_char){
    const Parsing::Settings settings = {',', '\\', '\n'};
    const size_t skip_first_lines_count = 0;

    Parsing::CSVParser<char, char, char> parser("../tests/csv_files/11.csv", settings,
                                                skip_first_lines_count);

    std::vector<std::tuple<char, char, char>> expected_tuples = {
            {'q', 'h', 'w'},
            {'g', 'n', 'm'},
            {}
    };

    auto it_expected = expected_tuples.begin();
    auto it = parser.begin();

    EXPECT_EQ(*it_expected, *it);

    EXPECT_NO_THROW(it_expected++);
    EXPECT_NO_THROW(it++);
    EXPECT_EQ(*it_expected, *it);

    EXPECT_THROW(it++, Exceptions::IncorrectCSVContentError);

    EXPECT_THROW(*parser.end(), Exceptions::ValueOfEndIteratorError);
    EXPECT_THROW(parser.end()++, Exceptions::IteratorOutOfRangeError);
}

TEST(CSVParserTests, test_independent_iterators_1){
    const Parsing::Settings settings = {',', '\\', '\n'};
    const size_t skip_first_lines_count = 0;

    Parsing::CSVParser<int, int> parser("../tests/csv_files/15.csv", settings,
                                        skip_first_lines_count);

    const std::tuple<int, int> line_1 = {1, 2};
    const std::tuple<int, int> line_2 = {3, 4};
    const std::tuple<int, int> line_3 = {5, 6};
    const std::tuple<int, int> line_4 = {7, 8};
    const std::tuple<int, int> line_5 = {9, 10};

    auto it1 = parser.begin();
    auto it2 = parser.begin();

    EXPECT_EQ(it1, it2);
    EXPECT_EQ(*it1, *it2);

    it1 += 2;
    EXPECT_EQ(*it1, line_3);
    EXPECT_EQ(*it2, line_1);

    it2++;
    EXPECT_EQ(*it1, line_3);
    EXPECT_EQ(*it2, line_2);

    it2 += 3;
    EXPECT_EQ(*it1, line_3);
    EXPECT_EQ(*it2, line_5);

    it1++;
    EXPECT_EQ(*it1, line_4);
    EXPECT_EQ(*it2, line_5);

    it1++;
    EXPECT_EQ(*it1, line_5);
    EXPECT_EQ(*it2, line_5);

    it1++;
    it2++;

    EXPECT_EQ(it1, parser.end());
    EXPECT_EQ(it2, parser.end());

    EXPECT_THROW(it1++, Exceptions::IteratorOutOfRangeError);
    EXPECT_THROW(it2++, Exceptions::IteratorOutOfRangeError);

    EXPECT_THROW(*it1, Exceptions::ValueOfEndIteratorError);
    EXPECT_THROW(*it1, Exceptions::ValueOfEndIteratorError);
}

TEST(CSVParserTests, test_reverse_iterators_int){
    const Parsing::Settings settings = {',', '\\', '\n'};
    const size_t skip_first_lines_count = 0;

    Parsing::CSVParser<int> parser("../tests/csv_files/2.csv", settings, skip_first_lines_count);

    std::vector<std::tuple<int>> expected_tuples = {
            {1},
            {2},
            {3},
            {4}
    };

    auto it_expected = expected_tuples.rbegin();
    auto it = parser.rbegin();

    for (; it != parser.rend(); it_expected++, it++){
        EXPECT_EQ(*it_expected, *it);
    }

    EXPECT_THROW(*it, Exceptions::ValueOfEndIteratorError);
    EXPECT_THROW(it++, Exceptions::IteratorOutOfRangeError);

    EXPECT_THROW(*parser.rend(), Exceptions::ValueOfEndIteratorError);
    EXPECT_THROW(parser.rend()++, Exceptions::IteratorOutOfRangeError);
}

TEST(CSVParserTests, test_reverse_iterators_int_int){
    const Parsing::Settings settings = {',', '\\', '\n'};
    const size_t skip_first_lines_count = 0;

    Parsing::CSVParser<int, int> parser("../tests/csv_files/6.csv", settings, skip_first_lines_count);

    std::vector<std::tuple<int, int>> expected_tuples = {
            {1, 2},
            {3, 4},
            {5, 6},
            {7, 8}
    };

    auto it_expected = expected_tuples.rbegin();
    auto it = parser.rbegin();

    for (; it != parser.rend(); it_expected++, it++){
        EXPECT_EQ(*it_expected, *it);
    }

    EXPECT_THROW(*it, Exceptions::ValueOfEndIteratorError);
    EXPECT_THROW(it++, Exceptions::IteratorOutOfRangeError);

    EXPECT_THROW(*parser.rend(), Exceptions::ValueOfEndIteratorError);
    EXPECT_THROW(parser.rend()++, Exceptions::IteratorOutOfRangeError);
}

TEST(CSVParserTests, test_reverse_iterators_int_int_skip){
    const Parsing::Settings settings = {',', '\\', '\n'};
    const size_t skip_first_lines_count = 2;

    Parsing::CSVParser<int, int> parser("../tests/csv_files/7.csv", settings, skip_first_lines_count);

    std::vector<std::tuple<int, int>> expected_tuples = {
            {5, 6},
            {7, 8}
    };

    auto it_expected = expected_tuples.rbegin();
    auto it = parser.rbegin();

    for (; it != parser.rend(); it_expected++, it++){
        EXPECT_EQ(*it_expected, *it);
    }

    EXPECT_THROW(*it, Exceptions::ValueOfEndIteratorError);
    EXPECT_THROW(it++, Exceptions::IteratorOutOfRangeError);

    EXPECT_THROW(*parser.rend(), Exceptions::ValueOfEndIteratorError);
    EXPECT_THROW(parser.rend()++, Exceptions::IteratorOutOfRangeError);
}

TEST(CSVParserTests, test_independent_reverse_iterators){
    const Parsing::Settings settings = {',', '\\', '\n'};
    const size_t skip_first_lines_count = 0;

    Parsing::CSVParser<int, int> parser("../tests/csv_files/15.csv", settings, skip_first_lines_count);

    const std::tuple<int, int> line_1 = {1, 2};
    const std::tuple<int, int> line_2 = {3, 4};
    const std::tuple<int, int> line_3 = {5, 6};
    const std::tuple<int, int> line_4 = {7, 8};
    const std::tuple<int, int> line_5 = {9, 10};

    auto it1 = parser.rbegin();
    auto it2 = parser.rbegin();

    EXPECT_EQ(it1, it2);
    EXPECT_EQ(*it1, *it2);

    it1 += 2;
    EXPECT_EQ(*it1, line_3);
    EXPECT_EQ(*it2, line_5);

    it2++;
    EXPECT_EQ(*it1, line_3);
    EXPECT_EQ(*it2, line_4);

    it2 += 3;
    EXPECT_EQ(*it1, line_3);
    EXPECT_EQ(*it2, line_1);

    it1++;
    EXPECT_EQ(*it1, line_2);
    EXPECT_EQ(*it2, line_1);

    it1++;
    EXPECT_EQ(*it1, line_1);
    EXPECT_EQ(*it2, line_1);

    it1++;
    it2++;

    EXPECT_EQ(it1, parser.rend());
    EXPECT_EQ(it2, parser.rend());

    EXPECT_THROW(it1++, Exceptions::IteratorOutOfRangeError);
    EXPECT_THROW(it2++, Exceptions::IteratorOutOfRangeError);

    EXPECT_THROW(*it1, Exceptions::ValueOfEndIteratorError);
    EXPECT_THROW(*it1, Exceptions::ValueOfEndIteratorError);
}

TEST(CSVParserTests, test_reverse_iterator_with_unusual_settings){
    const Parsing::Settings settings = {'-', '"', '#'};
    const size_t skip_first_lines_count = 0;

    Parsing::CSVParser<std::string, float, int> parser("../tests/csv_files/9.csv", settings,
                                                       skip_first_lines_count);

    std::vector<std::tuple<std::string, float, int>> expected_tuples = {
            {"Hello, world!", -0.5, -13},
            {"Wow - it works!", -1.0, -2}
    };

    auto it_expected = expected_tuples.rbegin();
    auto it = parser.rbegin();

    for (; it != parser.rend(); it_expected++, it++){
        EXPECT_EQ(*it_expected, *it);
    }

    EXPECT_THROW(*it, Exceptions::ValueOfEndIteratorError);
    EXPECT_THROW(it++, Exceptions::IteratorOutOfRangeError);

    EXPECT_THROW(*parser.end(), Exceptions::ValueOfEndIteratorError);
    EXPECT_THROW(parser.end()++, Exceptions::IteratorOutOfRangeError);
}

TEST(CSVParserTests, test_independent_forward_and_backward_iterators){
    const Parsing::Settings settings = {',', '\\', '\n'};
    const size_t skip_first_lines_count = 0;

    Parsing::CSVParser<int> parser("../tests/csv_files/18.csv", settings,
                                                       skip_first_lines_count);

    std::vector<std::tuple<int>> expected_tuples;
    for (int i = 1; i <= 10; i++){
        expected_tuples.emplace_back(i);
    }

    auto forward_file = parser.begin();
    auto backward_file = parser.rbegin();

    auto forward_vector = expected_tuples.begin();
    auto backward_vector = expected_tuples.rbegin();

    EXPECT_EQ(*forward_file, *forward_vector);
    EXPECT_EQ(*backward_file, *backward_vector);

    forward_file++;
    forward_vector++;
    backward_file++;
    backward_vector++;

    EXPECT_EQ(*forward_file, *forward_vector);
    EXPECT_EQ(*backward_file, *backward_vector);

    forward_file += 2;
    forward_vector += 2;
    backward_file += 3;
    backward_vector += 3;

    EXPECT_EQ(*forward_file, *forward_vector);
    EXPECT_EQ(*backward_file, *backward_vector);

    forward_file += 3;
    forward_vector += 3;
    backward_file += 5;
    backward_vector += 5;

    EXPECT_EQ(*forward_file, *forward_vector);
    EXPECT_EQ(*backward_file, *backward_vector);

    forward_file += 3;
    forward_vector += 3;

    EXPECT_EQ(*forward_file, *forward_vector);
    EXPECT_EQ(*backward_file, *backward_vector);

    forward_file += 1;
    backward_file += 1;

    EXPECT_EQ(forward_file, parser.end());
    EXPECT_EQ(backward_file, parser.rend());

    EXPECT_THROW(*forward_file, Exceptions::ValueOfEndIteratorError);
    EXPECT_THROW(*backward_file, Exceptions::ValueOfEndIteratorError);

    EXPECT_THROW(forward_file++, Exceptions::IteratorOutOfRangeError);
    EXPECT_THROW(backward_file++, Exceptions::IteratorOutOfRangeError);
}

TEST(CSVParserTests, test_out_of_range){
    const Parsing::Settings settings = {',', '\\', '\n'};
    const size_t skip_first_lines_count = 0;

    Parsing::CSVParser<int> parser("../tests/csv_files/19.csv", settings,
                                   skip_first_lines_count);

    auto forward = parser.begin();
    forward++;
    auto it1 = forward;
    auto it2 = forward;
    auto it3 = forward;
    EXPECT_THROW(it1 += 3, Exceptions::IteratorOutOfRangeError);
    EXPECT_THROW(it2 += 4, Exceptions::IteratorOutOfRangeError);
    EXPECT_THROW(it3 += 100, Exceptions::IteratorOutOfRangeError);

    auto backward = parser.rbegin();
    backward++;
    auto it4 = backward;
    auto it5 = backward;
    auto it6 = backward;
    EXPECT_THROW(it4 += 3, Exceptions::IteratorOutOfRangeError);
    EXPECT_THROW(it5 += 4, Exceptions::IteratorOutOfRangeError);
    EXPECT_THROW(it6 += 100, Exceptions::IteratorOutOfRangeError);
}

TEST(CSVParserTests, test_forward_empty_line){
    const Parsing::Settings settings = {',', '\\', '\n'};
    const size_t skip_first_lines_count = 0;

    Parsing::CSVParser<int, int> parser("../tests/csv_files/20.csv", settings,
                                   skip_first_lines_count);

    std::vector<std::tuple<int, int>> expected_tuples{
            {1, 2},
            {4, 5}
    };

    auto it_file = parser.begin();
    auto it_expected = expected_tuples.begin();

    for (; it_expected != expected_tuples.end(); it_file++, it_expected++){
        EXPECT_EQ(*it_file, *it_expected);
    }

    EXPECT_THROW(it_file++, Exceptions::IteratorOutOfRangeError);
    EXPECT_THROW(*it_file, Exceptions::ValueOfEndIteratorError);
}

TEST(CSVParserTests, test_backward_empty_line){
    const Parsing::Settings settings = {',', '\\', '\n'};
    const size_t skip_first_lines_count = 0;

    Parsing::CSVParser<int, int> parser("../tests/csv_files/20.csv", settings,
                                        skip_first_lines_count);

    std::vector<std::tuple<int, int>> expected_tuples{
            {1, 2},
            {4, 5}
    };

    auto it_file = parser.rbegin();
    auto it_expected = expected_tuples.rbegin();

    for (; it_expected != expected_tuples.rend(); it_file++, it_expected++){
        EXPECT_EQ(*it_file, *it_expected);
    }

    EXPECT_THROW(it_file++, Exceptions::IteratorOutOfRangeError);
    EXPECT_THROW(*it_file, Exceptions::ValueOfEndIteratorError);
}
