#pragma once

#include <iostream>
#include <limits>
#include <vector>

class MatrixChain {
   private:
    std::vector<int> dimensions;  // Вектор для хранения размеров матриц
    std::vector<std::vector<int>> m;  // Таблица для хранения минимального количества операций
    std::vector<std::vector<int>> s;  // Таблица для хранения оптимальных разделений

   public:
    void add_matrix(int rows, int cols) {
        if (!dimensions.empty() && dimensions.back() != rows) {
            throw std::invalid_argument("Incompatible matrix dimensions.");
        }
        if (dimensions.empty()) {
            dimensions.push_back(rows);
        }
        dimensions.push_back(cols);
    }

    void compute_optimal_order() {
        int matrix_n = dimensions.size() - 1;
        m.assign(matrix_n, std::vector<int>(matrix_n, 0));  // Инициализация таблицы стоимости операций
        s.assign(matrix_n, std::vector<int>(matrix_n, -1));  // Инициализация таблицы разделений

        for (int subsequence_length = 2; subsequence_length <= matrix_n; ++subsequence_length) {  // l - длина подпоследовательности
            for (int i = 0; i < matrix_n - subsequence_length + 1; ++i) {
                int j = i + subsequence_length - 1;
                m[i][j] = std::numeric_limits<int>::max();  // Устанавливаем максимальное значение
                for (int k = i; k < j; ++k) {
                    // Вычисляем количество операций для разделения в точке k
                    int q = m[i][k] + m[k + 1][j] +
                            dimensions[i] * dimensions[k + 1] * dimensions[j + 1];
                    if (q < m[i][j]) {
                        m[i][j] = q;
                        s[i][j] = k;  // Запоминаем точку разделения
                    }
                }
            }
        }
    }

    void get_optimal_order(int i, int j, std::vector<int> &order) {
        if (i == j) return;
        get_optimal_order(i, s[i][j], order);
        get_optimal_order(s[i][j] + 1, j, order);
        order.push_back(s[i][j]);
    }

    int get_optimal_multiplications() { return m[0][dimensions.size() - 2]; }

    int naive_order_multiplications() {
        int multiplications = 0;
        for (size_t i = 0; i < dimensions.size() - 2; ++i) {
            multiplications += dimensions[i] * dimensions[i + 1] * dimensions[i + 2];
        }
        return multiplications;
    }

    void compare_methods() {
        compute_optimal_order();
        int optimalMultiplications = get_optimal_multiplications();
        int naiveMultiplications = naive_order_multiplications();
        double improvement = (double)naiveMultiplications / optimalMultiplications;

        std::vector<int> order;
        get_optimal_order(0, dimensions.size() - 2, order);

        for (size_t i = 0; i < order.size(); ++i) {
            if (i > 0) std::cout << " ";
            std::cout << order[i];
        }
        //std::cout << "\nImprovement factor: " << improvement << std::endl;
    }
};
