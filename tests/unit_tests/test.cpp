#include <gtest/gtest.h>

#include "matrix.hpp"

TEST(matrix_buffer, ctor) {
    Matrix<double> matrix = {2, 3, {1, 2, 3, 4, 6, 7}};
    Matrix<double> lhs = matrix;

    auto it = matrix.begin();
    EXPECT_EQ(*(++it), 2);
}

TEST(matrix_buffer, multiply) {
    Matrix<double> matrix = {2, 3, {1, 2, 3, 4, 6, 7}};
    Matrix<double> lhs =
    { 3,
      2,
      {3, 5, 7, 1, 2, 3} };

    Matrix<double>
        mul = multiply<double>(matrix, lhs);

    EXPECT_EQ(mul[0][0], 23);
    EXPECT_EQ(mul[0][1], 16);
    EXPECT_EQ(mul[ 1][0], 68);
    EXPECT_EQ(mul[1][1], 47);
}