#include "gtest/gtest.h"
#include "../big_int.h"

TEST(logic_operations_test, conjunction1){
    BigInt a = BigInt(0);
    BigInt b = BigInt(0);
    EXPECT_EQ(a & b, BigInt(0));
}
TEST(logic_operations_test, conjunction2){
    BigInt a = BigInt(0);
    BigInt b = BigInt(15);
    EXPECT_EQ(a & b, BigInt(0));
}
TEST(logic_operations_test, conjunction3){
    BigInt a = BigInt(0);
    BigInt b = BigInt("11111111111111111111111");
    EXPECT_EQ(a & b, BigInt(0));
}
TEST(logic_operations_test, conjunction4){
    BigInt a = BigInt(1);
    BigInt b = BigInt(1);
    EXPECT_EQ(a & b, BigInt(1));
}
TEST(logic_operations_test, conjunction5){
    BigInt a = BigInt(1);
    BigInt b = BigInt(-1);
    EXPECT_EQ(a & b, BigInt(-1));
}
TEST(logic_operations_test, conjunction6){
    BigInt a = BigInt(-1);
    BigInt b = BigInt(-1);
    EXPECT_EQ(a & b, BigInt(1));
}
TEST(logic_operations_test, conjunction7){
    BigInt a = BigInt("11111111111");
    BigInt b = BigInt("22222222222");
    EXPECT_EQ(a & b, BigInt("67375494"));
}

TEST(logic_operations_test, disjunction1){
    BigInt a = BigInt(0);
    BigInt b = BigInt(0);
    EXPECT_EQ(a | b, BigInt(0));
}
TEST(logic_operations_test, disjunction2){
    BigInt a = BigInt(0);
    BigInt b = BigInt(1);
    EXPECT_EQ(a | b, BigInt(1));
}
TEST(logic_operations_test, disjunction3){
    BigInt a = BigInt(1);
    BigInt b = BigInt(1);
    EXPECT_EQ(a | b, BigInt(1));
}
TEST(logic_operations_test, disjunction4){
    BigInt a = BigInt(1);
    BigInt b = BigInt(-1);
    EXPECT_EQ(a | b, BigInt(-1));
}
TEST(logic_operations_test, disjunction5){
    BigInt a = BigInt(-1);
    BigInt b = BigInt(-1);
    EXPECT_EQ(a | b, BigInt(1));
}
TEST(logic_operations_test, disjunction6){
    BigInt a = BigInt("11111111111");
    BigInt b = BigInt("22222222222");
    EXPECT_EQ(a | b, BigInt("33265957839"));
}

TEST(logic_operations_test, xor1){
    BigInt a = BigInt(0);
    BigInt b = BigInt(0);
    EXPECT_EQ(a ^ b, BigInt(0));
}
TEST(logic_operations_test, xor2){
    BigInt a = BigInt(1);
    BigInt b = BigInt(0);
    EXPECT_EQ(a ^ b, BigInt(1));
}
TEST(logic_operations_test, xor3){
    BigInt a = BigInt(1);
    BigInt b = BigInt(1);
    EXPECT_EQ(a ^ b, BigInt(0));
}
TEST(logic_operations_test, xor4){
    BigInt a = BigInt(1);
    BigInt b = BigInt(2);
    EXPECT_EQ(a ^ b, BigInt(3));
}
TEST(logic_operations_test, xor5){
    BigInt a = BigInt(-1);
    BigInt b = BigInt(-2);
    EXPECT_EQ(a ^ b, BigInt(3));
}
TEST(logic_operations_test, xor6){
    BigInt a = BigInt(1);
    BigInt b = BigInt(-2);
    EXPECT_EQ(a ^ b, BigInt(-3));
}
TEST(logic_operations_test, xor7){
    BigInt a = BigInt("11111111111");
    BigInt b = BigInt("22222222222");
    EXPECT_EQ(a ^ b, BigInt("33198582345"));
}

TEST(logic_operations_test, inversion1){
    BigInt a = BigInt(0);
    EXPECT_EQ(~a, BigInt(std::to_string(BASE - 1)));
}
TEST(logic_operations_test, inversion2){
    BigInt a = BigInt(1);
    EXPECT_EQ(~a, BigInt(std::to_string(BASE - 2)));
}
TEST(logic_operations_test, inversion3){
    BigInt a = BigInt(-1);
    EXPECT_EQ(-(~a) , BigInt(std::to_string(BASE - 2)));
}
TEST(logic_operations_test, inversion4){
    BigInt a = BigInt(std::to_string(BASE - 1));
    EXPECT_EQ(~a , BigInt(0));
}
