#include "gtest/gtest.h"
#include "../big_int.h"

TEST(to_int_test, to_int_test1){
    BigInt b = BigInt();
    EXPECT_EQ(int(b), 0);
}
TEST(to_int_test, to_int_test2){
    BigInt b = BigInt(1);
    EXPECT_EQ(int(b), 1);
}
TEST(to_int_test, to_int_test3){
    BigInt b = BigInt(-1);
    EXPECT_EQ(int(b), -1);
}
TEST(to_int_test, to_int_test4){
    BigInt b = BigInt(INT_MAX - 1);
    EXPECT_EQ(int(b), INT_MAX - 1);
}
TEST(to_int_test, to_int_test5){
    BigInt b = BigInt(std::to_string(INT_MIN + 1));
    EXPECT_EQ(int(b), INT_MIN + 1);
}
TEST(to_int_test, to_int_test6){
    BigInt b = BigInt("111111111111111111111111111111111111");
    EXPECT_EQ(int(b), INT_MAX);
}
TEST(to_int_test, to_int_test7){
    BigInt b = BigInt("-111111111111111111111111111111111111");
    EXPECT_EQ(int(b), INT_MIN);
}
