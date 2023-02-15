#include "gtest/gtest.h"
#include "../big_int.h"

inline const unsigned int POINTER_SIZE = sizeof(void*);

TEST(size_test, size_test1){
    BigInt b = BigInt();
    const unsigned int real_size = b.size();
    const unsigned int expected_size = sizeof(bool) + sizeof(Block) * 1 + sizeof(int) + POINTER_SIZE;
    EXPECT_EQ(real_size, expected_size);
}
TEST(size_test, size_test2){
    BigInt b = BigInt(1);
    const unsigned int real_size = b.size();
    const unsigned int expected_size = sizeof(bool) + sizeof(Block) * 1 + sizeof(int) + POINTER_SIZE;
    EXPECT_EQ(real_size, expected_size);
}
TEST(size_test, size_test3){
    BigInt b = BigInt(std::to_string(BASE - 1));
    const unsigned int real_size = b.size();
    const unsigned int expected_size = sizeof(bool) + sizeof(Block) * 1 + sizeof(int) + POINTER_SIZE;
    EXPECT_EQ(real_size, expected_size);
}
TEST(size_test, size_test4){
    BigInt b = BigInt(std::to_string(BASE));
    const unsigned int real_size = b.size();
    const unsigned int expected_size = sizeof(bool) + sizeof(Block) * 2 + sizeof(int) + POINTER_SIZE;
    EXPECT_EQ(real_size, expected_size);
}
TEST(size_test, size_test5){
    BigInt b = BigInt(std::to_string(BASE + 1));
    const unsigned int real_size = b.size();
    const unsigned int expected_size = sizeof(bool) + sizeof(Block) * 2 + sizeof(int) + POINTER_SIZE;
    EXPECT_EQ(real_size, expected_size);
}
TEST(size_test, size_test6){
    BigInt b = BigInt(std::to_string(BASE * BASE - 1));
    const unsigned int real_size = b.size();
    const unsigned int expected_size = sizeof(bool) + sizeof(Block) * 2 + sizeof(int) + POINTER_SIZE;
    EXPECT_EQ(real_size, expected_size);
}
