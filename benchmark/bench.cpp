#include <benchmark/benchmark.h>
#include <fstream>
#include <stdexcept>
#include "matrix_chain.hpp"

auto init_matrix_chain_class() {
    std::ifstream file("../benchmark/data/test3.in");
    if (!file) {
        throw std::runtime_error("Reading file error");
    }

    std::size_t size;
    file >> size;
    if (!file.good()) {
        throw std::runtime_error ( "Invalid size" );
    }

    std::vector<std::size_t> dimensions(size);
    for (std::size_t i = 0; i < size; ++i) {
        file >> dimensions[i];
        if (!file.good()) {
            throw std::runtime_error ( "Invalid value" );
        }
    }

    MatrixChain<std::size_t> mc;
    for (int i = 0; i < size - 1; ++i) {
        mc.add_matrix(dimensions[i], dimensions[i + 1]);
    }

    mc.optimal_order_multiplications();

    return mc;
}

static void BM_Optimal(benchmark::State& state) {
    try {
        auto mc = init_matrix_chain_class();
        for (auto _ : state) {
            mc.get_optimal_multiplications();
        }
    } catch (const std::exception& e) {
        std::cerr << "Error : " << e.what() << '\n';
    } catch (...) {
        std::cerr << "Caught unknown exception\n";
    }
}

static void BM_Naive(benchmark::State& state) {
    try {
        auto mc = init_matrix_chain_class();
        for (auto _ : state) {
            mc.naive_order_multiplications();
        }
    } catch (const std::exception& e) {
        std::cerr << "Error : " << e.what() << '\n';
    } catch (...) {
        std::cerr << "Caught unknown exception\n";
    }
}

BENCHMARK(BM_Optimal)->Iterations(1);
BENCHMARK(BM_Naive)->Iterations(1);

BENCHMARK_MAIN();