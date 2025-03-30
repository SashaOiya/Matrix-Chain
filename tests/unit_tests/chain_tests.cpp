#include <gtest/gtest.h>

#include "matrix_chain.hpp"

TEST(matrix_chain, init_list_ctor) {
    MatrixChain<int> chain = { Matrix<int>(2, 3), Matrix<int>(4,2) };
    auto it = chain.begin();
    EXPECT_EQ(it->n_cols(), 3);
}

TEST(matrix_chain, copy_ctor) {
    MatrixChain<double> chain = { Matrix<double>{1, 3, {2, 4, 5, 6}}};
    MatrixChain<double> lhs = chain;

    EXPECT_TRUE(chain.size() == lhs.size());
    auto cols_ = chain[0].n_cols(), rows_ = chain[0].n_rows();
    for (size_t i = 0; i < cols_; ++i)
        for (auto j = 0; j < rows_; ++j) EXPECT_EQ(chain[0][j][i], lhs[0][j][i]);
}

TEST(matrix_chain, copy_assignment) {
    MatrixChain<double> chain = { Matrix<double>{1, 3, {2, 4, 5, 6}}};
    MatrixChain<double> lhs = {};
    lhs = chain;

    EXPECT_TRUE(chain.size() == lhs.size());
    auto cols_ = chain[0].n_cols(), rows_ = chain[0].n_rows();
    for (size_t i = 0; i < cols_; ++i)
        for (auto j = 0; j < rows_; ++j) EXPECT_EQ(chain[0][j][i], lhs[0][j][i]);
}

TEST(matrix_chain, move_ctor) {
    MatrixChain<double> chain = {Matrix<double>{1, 3, {2, 4, 5}}};
    std::vector<std::vector<double>> data = {};

    const auto cols_ = chain[0].n_cols(), rows_ = chain[0].n_rows();
    for (size_t i = 0; i < rows_; ++i) {
        std::vector<double> row = {};
        for (auto j = 0; j < cols_; ++j) row.push_back(chain[0][i][j]);
        data.push_back(row);
    }

    MatrixChain<double> lhs = std::move(chain);

    EXPECT_TRUE(data[0].size() == lhs[0].size());
    for (size_t i = 0; i < cols_; ++i)
        for (auto j = 0; j < rows_; ++j) EXPECT_EQ(data[j][i], lhs[0][j][i]);
}

TEST(matrix_chain, move_assignment) {
    MatrixChain<double> chain = {Matrix<double>{1, 3, {2, 4, 5}}};
    std::vector<std::vector<double>> data = {};

    const auto cols_ = chain[0].n_cols(), rows_ = chain[0].n_rows();
    for (size_t i = 0; i < rows_; ++i) {
        std::vector<double> row = {};
        for (auto j = 0; j < cols_; ++j) row.push_back(chain[0][i][j]);
        data.push_back(row);
    }

    MatrixChain<double> lhs = {};
    lhs = std::move(chain);

    EXPECT_TRUE(data[0].size() == lhs[0].size());
    for (size_t i = 0; i < cols_; ++i)
        for (auto j = 0; j < rows_; ++j) EXPECT_EQ(data[j][i], lhs[0][j][i]);
}