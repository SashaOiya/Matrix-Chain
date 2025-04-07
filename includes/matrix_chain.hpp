#pragma once

#include <cassert>
#include <cstddef>
#include <iostream>
#include <limits>
#include <stack>
#include <stdexcept>
#include <vector>

#include "matrix.hpp"

template <typename T>
class MatrixChain final {
    std::deque<Matrix<T>> chain_;
    std::deque<std::size_t> dimensions_;
    std::vector<std::vector<std::size_t>> min_operation;
    std::vector<std::vector<std::size_t>> optimal_splits;

    enum class InPos { Front, Back };

   public:
    using value_type = Matrix<T>;
    using size_type = std::size_t;
    using reference = value_type &;
    using const_reference = const value_type &;
    using iterator = std::deque<value_type>::iterator;
    using const_iterator = std::deque<value_type>::const_iterator;
    using reverse_iterator = std::deque<value_type>::reverse_iterator;
    using const_reverse_iterator = std::deque<value_type>::const_reverse_iterator;

    MatrixChain() = default;

    template <typename Iter>
    MatrixChain(Iter begin, Iter end) {
        chain_.insert(chain_.begin(), begin, end);

        if (size_type size = this->size()) {
            dimensions_.push_back(chain_[0].n_cols());
            dimensions_.push_back(chain_[0].n_rows());

            for (size_type i = 1; i < size - 1; ++i) {
                dimensions_.push_back(chain_[i].n_rows());
            }
        }
    }

    MatrixChain(std::initializer_list<value_type> l) : MatrixChain(l.begin(), l.end()) {}

    //----------------------------------------------
    size_type size() const noexcept { return chain_.size(); }
    bool operator==(const_reference rhs) { return chain_ == rhs.chain_; }

    // 1. at()
    // 2. std::out_of_range
    reference operator[](size_type n) {
        if (n >= size()) {
            throw std::invalid_argument("Invalid index");
        }
        return chain_[n];
    }

    bool empty() const noexcept { return chain_.empty(); }

    // remove bounding checks or implement separate methods

    reference front() {
        check_chain_size_init();
        return chain_.front();
    }

    const_reference front() const {
        check_chain_size_init();
        return chain_.front();
    }

    reference back() {
        check_chain_size_init();
        return chain_.back();
    }

    const_reference back() const {
        check_chain_size_init();
        return chain_.back();
    }

    void push_back(const_reference rhs) {
        insert_dimension<InPos::Back>(rhs.n_cols(), rhs.n_rows());
        chain_.push_back(rhs);
    }

    void push_back(value_type &&rhs) {
        insert_dimension<InPos::Back>(rhs.n_cols(), rhs.n_rows());
        chain_.push_back(std::move(rhs));
    }

    void push_front(const_reference rhs) {
        insert_dimension<InPos::Front>(rhs.n_cols(), rhs.n_rows());
        chain_.push_front(rhs);
    }
    void push_front(value_type &&rhs) {
        insert_dimension<InPos::Front>(rhs.n_cols(), rhs.n_rows());
        chain_.push_front(std::move(rhs));
    }

    void pop_back() {
        check_chain_size_init();
        dimensions_.pop_back();  // clear
        chain_.pop_back();
    }

    void pop_front() {
        check_chain_size_init();
        dimensions_.pop_front();  // clear
        chain_.pop_front();
    }
    //----------------------------------------------

    iterator begin() noexcept { return chain_.begin(); }
    const_iterator begin() const noexcept { return chain_.begin(); }
    iterator cbegin() const noexcept { return begin(); }

    iterator end() noexcept { return chain_.end(); }
    const_iterator end() const noexcept { return chain_.end(); }
    const_iterator cend() const noexcept { return end(); }

    reverse_iterator rbegin() noexcept { return chain_.rbegin(); }
    const_reverse_iterator rbegin() const noexcept { return chain_.rbegin(); }
    const_reverse_iterator crbegin() const noexcept { return rbegin(); }

    reverse_iterator rend() noexcept { return chain_.rend(); }
    const_reverse_iterator rend() const noexcept { return chain_.rend(); }
    const_reverse_iterator crend() const noexcept { return rend(); }
    //----------------------------------------------

    void optimal_order_multiplications() {
        size_type matrix_n = dimensions_.size() - 1;

        min_operation.assign(matrix_n, std::vector<size_type>(matrix_n, 0));
        optimal_splits.assign(matrix_n, std::vector<size_type>(matrix_n, -1));

        for (size_type subsequence_length = 2; subsequence_length <= matrix_n;
             ++subsequence_length) {
            for (size_type i = 0; i < matrix_n - subsequence_length + 1; ++i) {
                size_type j = i + subsequence_length - 1;
                min_operation[i][j] = std::numeric_limits<size_type>::max();

                for (auto k = i; k < j; ++k) {
                    auto q = min_operation[i][k] + min_operation[k + 1][j] +
                            dimensions_[i] * dimensions_[k + 1] * dimensions_[j + 1];
                    if (q < min_operation[i][j]) {
                        min_operation[i][j] = q;
                        optimal_splits[i][j] = k;
                    }
                }
            }
        }
    }

    size_type naive_order_multiplications() const {
        size_type multiplications = 0;
        for (size_t i = 0; i < dimensions_.size() - 2; ++i) {
            multiplications += dimensions_[i] * dimensions_[i + 1] * dimensions_[i + 2];
        }
        return multiplications;
    }

    auto get_optimal_order(int i, int j) {
        std::stack<std::pair<int, int>> stk;
        std::vector<size_type> output;

        stk.push({i, j});

        check_optimal_order_init();

        while (!stk.empty()) {
            const auto [left, right] = stk.top();
            stk.pop();

            if (left >= right) continue;

            const auto k = optimal_splits[left][right];
            output.push_back(k);  // reverse order

            stk.push({left, k});
            stk.push({k + 1, right});
        }

        // std::ranges::reverse(output); return output;
        return std::vector<size_type>(output.rbegin(), output.rend());
    }

    int get_optimal_multiplications() {
        check_optimal_order_init();
        return min_operation[0][dimensions_.size() - 2];
    }

    void print_optimal_order() {
        check_optimal_order_init();

        std::vector<size_type> order = get_optimal_order(0, dimensions_.size() - 2);

        // fmt::println("{}", fmt::join(order));
        auto it = order.begin();
        std::cout << *it++;
        for (auto end = order.end(); it != end; ++it) {
            std::cout << ' ' << *it;
        }
        std::cout << '\n';
    }

    void compare_methods() {
        int optimalMultiplications = get_optimal_multiplications();
        int naiveMultiplications = naive_order_multiplications();
        double improvement = (double)naiveMultiplications / optimalMultiplications;
        std::cout << "\nImprovement factor: " << improvement << '\n';
    }

   private:
    //----------------------------------------------
    void check_optimal_order_init() {
        if (optimal_splits.empty()) {
            assert(!min_operation.empty());
            optimal_order_multiplications();
        }
        assert(!min_operation.empty());
    }

    void check_chain_size_init() {
        if (!size()) {
            throw std::runtime_error("Matrix chain is empty");
        }
    }

    void check_prev_matrix_size(const_reference rhs) {
        if (!chain_.empty() && back().n_cols != rhs.back().n_rows()) {
            throw std::invalid_argument("Incorrect new matrix size");
        }
    }
    //----------------------------------------------
    template <InPos position>
    void insert_dimension(size_type col, size_type row) {
        // maybe assert
        if (!dimensions_.empty() && dimensions_.back() != row) {
            throw std::invalid_argument("Incompatible matrix dimensions.");
        }
        if (dimensions_.empty()) {
            dimensions_.push_back(row);
            dimensions_.push_back(col);
        } else {
            if constexpr (position == InPos::Back) {
                dimensions_.push_back(col);
            } else {
                dimensions_.push_front(row);
            }
        }
    }
};
