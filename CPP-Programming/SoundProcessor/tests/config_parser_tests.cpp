#include "gtest/gtest.h"
#include "../src/config_parser.h"
#include "../src/custom_exceptions.h"

// incorrect path
TEST(ConfigsParserTests, test_incorrect_path){
    EXPECT_THROW(ConfigParsing::ConfigParser("<incorrect path>"),
                 Exceptions::IncorrectConfigError);
}

// incorrect path
TEST(ConfigsParserTests, test_empty_file){
    EXPECT_THROW(ConfigParsing::ConfigParser("../tests/config_files_for_tests/empty_config.txt"),
                 Exceptions::IncorrectConfigError);
}
