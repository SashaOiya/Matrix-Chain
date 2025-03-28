#include <gtest/gtest.h>

#include "matrix_chain.hpp"

TEST(matrix_chain, push_back) {
    std::vector<std::size_t> dimensions{30,35,15,5,10};
    MatrixChain<std::size_t> chain;

    const std::size_t size = dimensions.size();
    for (int i = 0; i < size - 1; ++i) {
        chain.push_back( Matrix<std::size_t>(dimensions[i], dimensions[i+1]));
    }
}
