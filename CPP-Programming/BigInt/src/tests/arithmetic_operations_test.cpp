#include "gtest/gtest.h"
#include "../big_int.h"

TEST(addition_test, addition_test_1){
    BigInt a = BigInt(0);
    BigInt b = BigInt(0);
    EXPECT_EQ(a + b, BigInt(0));
}
TEST(addition_test, addition_test_2){
    BigInt a = BigInt(0);
    BigInt b = BigInt(1);
    EXPECT_EQ(a + b, BigInt(1));
}
TEST(addition_test, addition_test_3){
    BigInt a = BigInt(1);
    BigInt b = BigInt(1);
    EXPECT_EQ(a + b, BigInt(2));
}
TEST(addition_test, addition_test_4){
    BigInt a = BigInt(99);
    BigInt b = BigInt(1);
    EXPECT_EQ(a + b, BigInt(100));
}
TEST(addition_test, addition_test_5){
    BigInt a = BigInt("111111111111111111111111111111111111111111111");
    BigInt b = BigInt("111111111111111111111111111111111111111111111");
    EXPECT_EQ(a + b, BigInt("222222222222222222222222222222222222222222222"));
}
TEST(addition_test, addition_test_6){
    BigInt a = BigInt(1);
    BigInt b = BigInt(-1);
    EXPECT_EQ(a + b, BigInt(0));
}
TEST(addition_test, addition_test_7){
    BigInt a = BigInt(-1);
    BigInt b = BigInt(-1);
    EXPECT_EQ(a + b, BigInt(-2));
}
TEST(addition_test, addition_test_8){
    BigInt a = BigInt(0);
    BigInt b = BigInt(-1);
    EXPECT_EQ(a + b, BigInt(-1));
}
TEST(addition_test, addition_test_9){
    BigInt a = BigInt(100);
    BigInt b = BigInt(-1);
    EXPECT_EQ(a + b, BigInt(99));
}
TEST(addition_test, addition_test_10){
    BigInt a = BigInt(1);
    BigInt b = BigInt(-100);
    EXPECT_EQ(a + b, BigInt(-99));
}
TEST(addition_test, addition_test_11){
    BigInt a = BigInt("111111111111111111111111111111111111111111111");
    BigInt b = BigInt("-111111111111111111111111111111111111111111111");
    EXPECT_EQ(a + b, BigInt(0));
}

TEST(subtraction_test, subtraction_test_1){
    BigInt a = BigInt(0);
    BigInt b = BigInt(0);
    EXPECT_EQ(a - b, BigInt(0));
}
TEST(subtraction_test, subtraction_test_2){
    BigInt a = BigInt(1);
    BigInt b = BigInt(0);
    EXPECT_EQ(a - b, BigInt(1));
}
TEST(subtraction_test, subtraction_test_3){
    BigInt a = BigInt(0);
    BigInt b = BigInt(1);
    EXPECT_EQ(a - b, BigInt(-1));
}
TEST(subtraction_test, subtraction_test_4){
    BigInt a = BigInt(1);
    BigInt b = BigInt(-1);
    EXPECT_EQ(a - b, BigInt(2));
}
TEST(subtraction_test, subtraction_test_5){
    BigInt a = BigInt(1);
    BigInt b = BigInt(1);
    EXPECT_EQ(a - b, BigInt(0));
}
TEST(subtraction_test, subtraction_test_6){
    BigInt a = BigInt(1000);
    BigInt b = BigInt(1);
    EXPECT_EQ(a - b, BigInt(999));
}
TEST(subtraction_test, subtraction_test_7){
    BigInt a = BigInt(-999);
    BigInt b = BigInt(1);
    EXPECT_EQ(a - b, BigInt(-1000));
}
TEST(subtraction_test, subtraction_test_8){
    BigInt a = BigInt("2222222222222222222222222222222222222222");
    BigInt b = BigInt("1111111111111111111111111111111111111111");
    EXPECT_EQ(a - b, BigInt("1111111111111111111111111111111111111111"));
}

TEST(multiplication_test, multiplication_test_1){
    BigInt a = BigInt(0);
    BigInt b = BigInt(0);
    EXPECT_EQ(a * b, BigInt(0));
}
TEST(multiplication_test, multiplication_test_2){
    BigInt a = BigInt(1);
    BigInt b = BigInt(0);
    EXPECT_EQ(a * b, BigInt(0));
}
TEST(multiplication_test, multiplication_test_3){
    BigInt a = BigInt(-1);
    BigInt b = BigInt(0);
    EXPECT_EQ(a * b, BigInt(0));
}
TEST(multiplication_test, multiplication_test_4){
    BigInt a = BigInt("1111111111111111111111111111111111111");
    BigInt b = BigInt(0);
    EXPECT_EQ(a * b, BigInt(0));
}
TEST(multiplication_test, multiplication_test_5){
    BigInt a = BigInt(1);
    BigInt b = BigInt(1);
    EXPECT_EQ(a * b, BigInt(1));
}
TEST(multiplication_test, multiplication_test_6){
    BigInt a = BigInt(1);
    BigInt b = BigInt(-1);
    EXPECT_EQ(a * b, BigInt(-1));
}
TEST(multiplication_test, multiplication_test_7){
    BigInt a = BigInt(1);
    BigInt b = BigInt("1111111111111111111111111111111111111111111111111");
    EXPECT_EQ(a * b, BigInt("1111111111111111111111111111111111111111111111111"));
}
TEST(multiplication_test, multiplication_test_8){
    BigInt a = BigInt(-1);
    BigInt b = BigInt("1111111111111111111111111111111111111111111111111");
    EXPECT_EQ(a * b, BigInt("-1111111111111111111111111111111111111111111111111"));
}
TEST(multiplication_test, multiplication_test_9){
    BigInt a = BigInt(-1);
    BigInt b = BigInt(-1);
    EXPECT_EQ(a * b, BigInt(1));
}
TEST(multiplication_test, multiplication_test_10){
    BigInt a = BigInt("99999999999");
    BigInt b = BigInt("1234567890");
    EXPECT_EQ(a * b, BigInt("123456788998765432110"));
}
TEST(multiplication_test, multiplication_test_11){
    BigInt a = BigInt("111111111111111111111111111111111");
    BigInt b = BigInt("99999999999999999999999999999999");
    EXPECT_EQ(a * b, BigInt("11111111111111111111111111111110988888888888888888888888888888889"));
}

TEST(division_test, division_test_test_1){
    BigInt a = BigInt(0);
    BigInt b = BigInt(1);
    EXPECT_EQ(a / b, BigInt(0));
}
TEST(division_test, division_test_test_2){
    BigInt a = BigInt(0);
    BigInt b = BigInt(-1);
    EXPECT_EQ(a / b, BigInt(0));
}
TEST(division_test, division_test_test_3){
    BigInt a = BigInt(0);
    BigInt b = BigInt(123456789);
    EXPECT_EQ(a / b, BigInt(0));
}
TEST(division_test, division_test_test_4){
    BigInt a = BigInt(0);
    BigInt b = BigInt(-123456789);
    EXPECT_EQ(a / b, BigInt(0));
}
TEST(division_test, division_test_test_5){
    BigInt a = BigInt(999);
    BigInt b = BigInt(1000);
    EXPECT_EQ(a / b, BigInt(0));
}
TEST(division_test, division_test_test_6){
    BigInt a = BigInt(-999);
    BigInt b = BigInt(1000);
    EXPECT_EQ(a / b, BigInt(0));
}
TEST(division_test, division_test_test_7){
    BigInt a = BigInt(999);
    BigInt b = BigInt(-1000);
    EXPECT_EQ(a / b, BigInt(0));
}
TEST(division_test, division_test_test_8){
    BigInt a = BigInt(123456789);
    BigInt b = BigInt(10);
    EXPECT_EQ(a / b, BigInt(12345678));
}
TEST(division_test, division_test_test_9){
    BigInt a = BigInt(100);
    BigInt b = BigInt(99);
    EXPECT_EQ(a / b, BigInt(1));
}
TEST(division_test, division_test_test_10){
    BigInt a = BigInt(1);
    BigInt b = BigInt(-1);
    EXPECT_EQ(a / b, BigInt(-1));
}
TEST(division_test, division_test_test_11){
    BigInt a = BigInt(1);
    BigInt b = BigInt(1);
    EXPECT_EQ(a / b, BigInt(1));
}
TEST(division_test, division_test_test_12){
    BigInt a = BigInt(-1);
    BigInt b = BigInt(-1);
    EXPECT_EQ(a / b, BigInt(1));
}
TEST(division_test, division_test_test_13){
    BigInt a = BigInt(-1);
    BigInt b = BigInt(1);
    EXPECT_EQ(a / b, BigInt(-1));
}
TEST(division_test, division_test_test_14){
    BigInt a = BigInt(-20);
    BigInt b = BigInt(4);
    EXPECT_EQ(a / b, BigInt(-5));
}
TEST(division_test, division_test_test_15){
    BigInt a = BigInt(42);
    BigInt b = BigInt(20);
    EXPECT_EQ(a / b, BigInt(2));
}
TEST(division_test, division_test_test_16){
    BigInt a = BigInt(993960);
    BigInt b = BigInt(99);
    EXPECT_EQ(a / b, BigInt(10040));
}
TEST(division_test, division_test_test_17){
    BigInt a = BigInt(100);
    BigInt b = BigInt(1);
    EXPECT_EQ(a / b, BigInt(100));
}
TEST(division_test, division_test_test_18){
    BigInt a = BigInt(0);
    BigInt b = BigInt(0);
    EXPECT_THROW(a / b, std::invalid_argument);
}
TEST(division_test, division_test_test_19){
    BigInt a = BigInt(1);
    BigInt b = BigInt(0);
    EXPECT_THROW(a / b, std::invalid_argument);
}
TEST(division_test, division_test_test_20){
    BigInt a = BigInt(-1);
    BigInt b = BigInt(0);
    EXPECT_THROW(a / b, std::invalid_argument);
}

TEST(division_test, division_test_test_21){
    BigInt a = BigInt(0);
    BigInt b = BigInt(1);
    EXPECT_EQ(a % b, BigInt(0));
}
TEST(division_test, division_test_test_22){
    BigInt a = BigInt(0);
    BigInt b = BigInt(-1);
    EXPECT_THROW(a % b, std::invalid_argument);
}
TEST(division_test, division_test_test_23){
    BigInt a = BigInt(0);
    BigInt b = BigInt(100);
    EXPECT_EQ(a % b, BigInt(0));
}
TEST(division_test, division_test_test_24){
    BigInt a = BigInt(1);
    BigInt b = BigInt(-1);
    EXPECT_THROW(a % b, std::invalid_argument);
}
TEST(division_test, division_test_test_25){
    BigInt a = BigInt(1000);
    BigInt b = BigInt(-1);
    EXPECT_THROW(a % b, std::invalid_argument);
}
TEST(division_test, division_test_test_26){
    BigInt a = BigInt(1);
    BigInt b = BigInt(1);
    EXPECT_EQ(a % b, BigInt(0));
}
TEST(division_test, division_test_test_27){
    BigInt a = BigInt(10000);
    BigInt b = BigInt(1);
    EXPECT_EQ(a % b, BigInt(0));
}
TEST(division_test, division_test_test_28){
    BigInt a = BigInt(-1);
    BigInt b = BigInt(-1);
    EXPECT_THROW(a % b, std::invalid_argument);
}
TEST(division_test, division_test_test_29){
    BigInt a = BigInt(25);
    BigInt b = BigInt(2);
    EXPECT_EQ(a % b, BigInt(1));
}
TEST(division_test, division_test_test_30){
    BigInt a = BigInt(123456789);
    BigInt b = BigInt(10);
    EXPECT_EQ(a % b, BigInt(9));
}
TEST(division_test, division_test_test_31){
    BigInt a = BigInt(65537);
    BigInt b = BigInt(257);
    EXPECT_EQ(a % b, BigInt(2));
}
TEST(division_test, division_test_test_32){
    BigInt a = BigInt("12345678901234567890");
    BigInt b = BigInt("99999999999");
    EXPECT_EQ(a % b, BigInt(1358024679));
}
TEST(division_test, division_test_test_33){
    BigInt a = BigInt(10000);
    BigInt b = BigInt(2);
    EXPECT_EQ(a % b, BigInt(0));
}
TEST(division_test, division_test_test_34){
    BigInt a = BigInt(-2);
    BigInt b = BigInt(3);
    EXPECT_EQ(a % b, BigInt(1));
}
