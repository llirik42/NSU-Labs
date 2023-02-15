#include "gtest/gtest.h"
#include "../src/matrix.h"

using namespace AllChoices;

// incorrect path
TEST(MatrixTest, test0){
    const char* path = "<incorrect path>";
    const Matrix matrix = Matrix(path);

    EXPECT_TRUE(matrix.has_error());
}

TEST(MatrixTest, test1){
    const char* path = "../matrices/matrix_1";
    const Matrix matrix = Matrix(path);

    EXPECT_TRUE(matrix.has_error());
}

TEST(MatrixTest, test2){
    const char* path = "../matrices/matrix_2";
    const Matrix matrix = Matrix(path);

    EXPECT_FALSE(matrix.has_error());
    EXPECT_TRUE(matrix.is_consistent());

    EXPECT_EQ(matrix[CCC], Row({7, 7, 7}));
    EXPECT_EQ(matrix[CCD], Row({3, 3, 9}));
    EXPECT_EQ(matrix[CDC], Row({3, 9, 3}));
    EXPECT_EQ(matrix[DCC], Row({9, 3, 3}));
    EXPECT_EQ(matrix[CDD], Row({0, 5, 5}));
    EXPECT_EQ(matrix[DCD], Row({5, 0, 5}));
    EXPECT_EQ(matrix[DDC], Row({5, 5, 0}));
    EXPECT_EQ(matrix[DDD], Row({1, 1, 1}));
}

TEST(MatrixTest, test3){
    const char* path = "../matrices/matrix_3";
    const Matrix matrix = Matrix(path);

    EXPECT_FALSE(matrix.has_error());
    EXPECT_TRUE(matrix.is_consistent());

    EXPECT_EQ(matrix[CCC], Row({8, 8, 8}));
    EXPECT_EQ(matrix[CCD], Row({6, 6, 10}));
    EXPECT_EQ(matrix[CDC], Row({6, 10, 6}));
    EXPECT_EQ(matrix[DCC], Row({10, 6, 6}));
    EXPECT_EQ(matrix[CDD], Row({4, 7, 7}));
    EXPECT_EQ(matrix[DCD], Row({7, 4, 7}));
    EXPECT_EQ(matrix[DDC], Row({7, 7, 4}));
    EXPECT_EQ(matrix[DDD], Row({5, 5, 5}));
}

TEST(MatrixTest, test4){
    const char* path = "../matrices/matrix_4";
    const Matrix matrix = Matrix(path);

    EXPECT_FALSE(matrix.has_error());
    EXPECT_TRUE(matrix.is_consistent());

    EXPECT_EQ(matrix[CCC], Row({-2, -2, -2}));
    EXPECT_EQ(matrix[CCD], Row({-4, -4, -1}));
    EXPECT_EQ(matrix[CDC], Row({-4, -1, -4}));
    EXPECT_EQ(matrix[DCC], Row({-1, -4, -4}));
    EXPECT_EQ(matrix[CDD], Row({-6, -3, -3}));
    EXPECT_EQ(matrix[DCD], Row({-3, -6, -3}));
    EXPECT_EQ(matrix[DDC], Row({-3, -3, -6}));
    EXPECT_EQ(matrix[DDD], Row({-5, -5, -5}));
}

TEST(MatrixTest, test5){
    const char* path = "../matrices/matrix_5";
    const Matrix matrix = Matrix(path);

    EXPECT_FALSE(matrix.has_error());
    EXPECT_TRUE(matrix.is_consistent());

    EXPECT_EQ(matrix[CCC], Row({2, 2, 2}));
    EXPECT_EQ(matrix[CCD], Row({-4, -4, 3}));
    EXPECT_EQ(matrix[CDC], Row({-4, 3, -4}));
    EXPECT_EQ(matrix[DCC], Row({3, -4, -4}));
    EXPECT_EQ(matrix[CDD], Row({-6, 0, 0}));
    EXPECT_EQ(matrix[DCD], Row({0, -6, 0}));
    EXPECT_EQ(matrix[DDC], Row({0, 0, -6}));
    EXPECT_EQ(matrix[DDD], Row({-5, -5, -5}));
}

TEST(MatrixTest, test6){
    const char* path = "../matrices/matrix_6";
    const Matrix matrix = Matrix(path);

    EXPECT_FALSE(matrix.has_error());
    EXPECT_FALSE(matrix.is_consistent());
}

TEST(MatrixTest, test7){
    const char* path = "../matrices/matrix_7";
    const Matrix matrix = Matrix(path);

    EXPECT_FALSE(matrix.has_error());
    EXPECT_FALSE(matrix.is_consistent());
}

TEST(MatrixTest, test8){
    const char* path = "../matrices/matrix_8";
    const Matrix matrix = Matrix(path);

    EXPECT_FALSE(matrix.has_error());
    EXPECT_FALSE(matrix.is_consistent());
}

TEST(MatrixTest, test9){
    const char* path = "../matrices/matrix_9";
    const Matrix matrix = Matrix(path);

    EXPECT_FALSE(matrix.has_error());
    EXPECT_TRUE(matrix.is_consistent());

    EXPECT_EQ(matrix[CCC], Row({8, 8, 8}));
    EXPECT_EQ(matrix[CCD], Row({6, 6, 10}));
    EXPECT_EQ(matrix[CDC], Row({6, 10, 6}));
    EXPECT_EQ(matrix[DCC], Row({10, 6, 6}));
    EXPECT_EQ(matrix[CDD], Row({4, 7, 7}));
    EXPECT_EQ(matrix[DCD], Row({7, 4, 7}));
    EXPECT_EQ(matrix[DDC], Row({7, 7, 4}));
    EXPECT_EQ(matrix[DDD], Row({5, 5, 5}));
}

TEST(MatrixTest, test10){
    const char* path = "../matrices/matrix_10";
    const Matrix matrix = Matrix(path);

    EXPECT_FALSE(matrix.has_error());
    EXPECT_TRUE(matrix.is_consistent());

    EXPECT_EQ(matrix[CCC], Row({2, 2, 2}));
    EXPECT_EQ(matrix[CCD], Row({-4, -4, 3}));
    EXPECT_EQ(matrix[CDC], Row({-4, 3, -4}));
    EXPECT_EQ(matrix[DCC], Row({3, -4, -4}));
    EXPECT_EQ(matrix[CDD], Row({-6, 0, 0}));
    EXPECT_EQ(matrix[DCD], Row({0, -6, 0}));
    EXPECT_EQ(matrix[DDC], Row({0, 0, -6}));
    EXPECT_EQ(matrix[DDD], Row({-5, -5, -5}));
}

TEST(MatrixTest, test11){
    const char* path = "../matrices/matrix_11";
    const Matrix matrix = Matrix(path);

    EXPECT_TRUE(matrix.has_error());
}

TEST(MatrixTest, test12){
    const char* path = "../matrices/matrix_12";
    const Matrix matrix = Matrix(path);

    EXPECT_TRUE(matrix.has_error());
}

TEST(MatrixTest, test13){
    const char* path = "../matrices/matrix_13";
    const Matrix matrix = Matrix(path);

    EXPECT_TRUE(matrix.has_error());
}

TEST(MatrixTest, test14){
    const char* path = "../matrices/matrix_14";
    const Matrix matrix = Matrix(path);

    EXPECT_TRUE(matrix.has_error());
}

TEST(MatrixTest, test15){
    const char* path = "../matrices/matrix_15";
    const Matrix matrix = Matrix(path);

    EXPECT_TRUE(matrix.has_error());
}

TEST(MatrixTest, test16){
    const char* path = "../matrices/matrix_16";
    const Matrix matrix = Matrix(path);

    EXPECT_TRUE(matrix.has_error());
}

TEST(MatrixTest, test17){
    const char* path = "../matrices/matrix_17";
    const Matrix matrix = Matrix(path);

    EXPECT_TRUE(matrix.has_error());
}

TEST(MatrixTest, test18){
    const char* path = "../matrices/matrix_18";
    const Matrix matrix = Matrix(path);

    EXPECT_TRUE(matrix.has_error());
}

TEST(MatrixTest, test19){
    const char* path = "../matrices/matrix_19";
    const Matrix matrix = Matrix(path);

    EXPECT_TRUE(matrix.has_error());
}

TEST(MatrixTest, test20){
    const char* path = "../matrices/matrix_20";
    const Matrix matrix = Matrix(path);

    EXPECT_TRUE(matrix.has_error());
}

TEST(MatrixTest, test21){
    const char* path = "../matrices/matrix_21";
    const Matrix matrix = Matrix(path);

    EXPECT_TRUE(matrix.has_error());
}

TEST(MatrixTest, test22){
    const char* path = "../matrices/matrix_22";
    const Matrix matrix = Matrix(path);

    EXPECT_TRUE(matrix.has_error());
}

TEST(MatrixTest, test23){
    const char* path = "../matrices/matrix_23";
    const Matrix matrix = Matrix(path);

    EXPECT_TRUE(matrix.has_error());
}

TEST(MatrixTest, test24){
    const char* path = "../matrices/matrix_24";
    const Matrix matrix = Matrix(path);

    EXPECT_TRUE(matrix.has_error());
}
TEST(MatrixTest, test25){
    const char* path = "../matrices/matrix_25";
    const Matrix matrix = Matrix(path);

    EXPECT_TRUE(matrix.has_error());
}

TEST(MatrixTest, test26){
    const char* path = "../matrices/matrix_26";
    const Matrix matrix = Matrix(path);

    EXPECT_TRUE(matrix.has_error());
}

TEST(MatrixTest, test27){
    const char* path = "../matrices/matrix_27";
    const Matrix matrix = Matrix(path);

    EXPECT_TRUE(matrix.has_error());
}

TEST(MatrixTest, test28){
    const char* path = "../matrices/matrix_28";
    const Matrix matrix = Matrix(path);

    EXPECT_TRUE(matrix.has_error());
}

TEST(MatrixTest, test29){
    const char* path = "../matrices/matrix_29";
    const Matrix matrix = Matrix(path);

    EXPECT_TRUE(matrix.has_error());
}

// Default matrix
TEST(MatrixTest, test30){
    const Matrix matrix = Matrix();

    EXPECT_FALSE(matrix.has_error());
    EXPECT_TRUE(matrix.is_consistent());

    EXPECT_EQ(matrix[CCC], Row({7, 7, 7}));
    EXPECT_EQ(matrix[CCD], Row({3, 3, 9}));
    EXPECT_EQ(matrix[CDC], Row({3, 9, 3}));
    EXPECT_EQ(matrix[DCC], Row({9, 3, 3}));
    EXPECT_EQ(matrix[CDD], Row({0, 5, 5}));
    EXPECT_EQ(matrix[DCD], Row({5, 0, 5}));
    EXPECT_EQ(matrix[DDC], Row({5, 5, 0}));
    EXPECT_EQ(matrix[DDD], Row({1, 1, 1}));
}
