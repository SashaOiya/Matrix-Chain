#include <iostream>
#include <vector>

#include "matrix_chain.hpp"

int main() try {
    std::size_t size;
    std::cin >> size;
    std::vector<std::size_t> dimensions(size);
    for (std::size_t i = 0; i < size; ++i) {
        std::cin >> dimensions[i];
    }

    MatrixChain mc;
    for (int i = 0; i < size - 1; ++i) {
        mc.add_matrix(dimensions[i], dimensions[i + 1]);
    }

    mc.compare_methods();  // Запуск анализа оптимального порядка умножения

    return 0;
} catch (const std::exception &e) {
    std::cerr << "Error : " << e.what() << '\n';
    return EXIT_FAILURE;
} catch (...) {
    std::cerr << "Caught unknown exception\n";
    return EXIT_FAILURE;
}