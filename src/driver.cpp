#include <iostream>
#include <stdexcept>
#include <vector>

#include "matrix_chain.hpp"

int main() try {
    std::size_t size;
    std::cin >> size;
    if (!std::cin.good()) {
        throw std::runtime_error ( "Invalid chain size");
    }
    std::vector<std::size_t> dimensions(size);
    for (std::size_t i = 0; i < size; ++i) {
        std::cin >> dimensions[i];
    }

    MatrixChain<std::size_t> chain;
    for (int i = 0; i < size - 1; ++i) {
        chain.push_back(Matrix<std::size_t>(dimensions[i], dimensions[i + 1]));
    }

    chain.print_optimal_order();
    //mc.compare_methods();

    return 0;
} catch (const std::exception &e) {
    std::cerr << "Error : " << e.what() << '\n';
    return EXIT_FAILURE;
} catch (...) {
    std::cerr << "Caught unknown exception\n";
    return EXIT_FAILURE;
}