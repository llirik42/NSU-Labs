#include "gtest/gtest.h"
#include "../big_int.h"

TEST(constructors_test, empty_constructor_test){
    BigInt b = BigInt();
    EXPECT_EQ(std::string(b), "0");
}
TEST(constructors_test, constructors_test1){
    BigInt b = BigInt(0);
    EXPECT_EQ(std::string(b), "0");
}
TEST(constructors_test, constructors_test2){
    BigInt b = BigInt("0");
    EXPECT_EQ(std::string(b), "0");
}
TEST(constructors_test, constructors_test3){
    BigInt b = BigInt(1234);
    EXPECT_EQ(std::string(b), "1234");
}
TEST(constructors_test, constructors_test4){
    BigInt b = BigInt(-1235);
    EXPECT_EQ(std::string(b), "-1235");
}
TEST(constructors_test, constructors_test5){
    BigInt b = BigInt("12345678901234567890");
    EXPECT_EQ(std::string(b), "12345678901234567890");
}
TEST(constructors_test, constructors_test6){
    BigInt b = BigInt("-12345678901234567890");
    EXPECT_EQ(std::string(b), "-12345678901234567890");
}
TEST(constructors_test, constructors_test7){
    BigInt b = BigInt("111111111111111111111111111111111111111111111111111111111111");
    EXPECT_EQ(std::string(b), "111111111111111111111111111111111111111111111111111111111111");
}
TEST(constructors_test, constructors_test8){
    BigInt b = BigInt("-111111111111111111111111111111111111111111111111111111111111");
    EXPECT_EQ(std::string(b), "-111111111111111111111111111111111111111111111111111111111111");
}
TEST(constructors_test, constructors_test9){
    EXPECT_THROW(BigInt b = BigInt("-0"), std::invalid_argument);
}
TEST(constructors_test, constructors_test10){
    EXPECT_THROW(BigInt b = BigInt("00"), std::invalid_argument);
}
TEST(constructors_test, constructors_test11){
    EXPECT_THROW(BigInt b = BigInt("-00"), std::invalid_argument);
}
TEST(constructors_test, constructors_test12){
    EXPECT_THROW(BigInt b = BigInt("01"), std::invalid_argument);
}
TEST(constructors_test, constructors_test13){
    EXPECT_THROW(BigInt b = BigInt("-01"), std::invalid_argument);
}
TEST(constructors_test, constructors_test14){
    EXPECT_THROW(BigInt b = BigInt("-2.5"), std::invalid_argument);
}
TEST(constructors_test, constructors_test15){
    EXPECT_THROW(BigInt b = BigInt("--0"), std::invalid_argument);
}
TEST(constructors_test, constructors_test16){
    EXPECT_THROW(BigInt b = BigInt("--1"), std::invalid_argument);
}
TEST(constructors_test, constructors_test17){
    EXPECT_THROW(BigInt b = BigInt("10.5"), std::invalid_argument);
}
TEST(constructors_test, constructors_test18){
    EXPECT_THROW(BigInt b = BigInt("4,5"), std::invalid_argument);
}
TEST(constructors_test, constructors_test19){
    EXPECT_THROW(BigInt b = BigInt("-6,7"), std::invalid_argument);
}
TEST(constructors_test, constructors_test20){
    EXPECT_THROW(BigInt b = BigInt("-123456789g"), std::invalid_argument);
}
TEST(constructors_test, constructors_test21){
    EXPECT_THROW(BigInt b = BigInt("-123456789-"), std::invalid_argument);
}
