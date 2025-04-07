#pragma once

#include <cstddef>
#include <stdexcept>
#include <utility>
#include <vector>

template <typename T>
class Matrix final : private std::vector<T> {
   private:
    std::size_t rows_;
    std::size_t cols_;

    using base = std::vector<T>;

   public:
    using value_type = T;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using typename base::const_iterator;
    using typename base::const_reverse_iterator;
    using typename base::iterator;
    using typename base::reverse_iterator;

    using base::data;

    Matrix(size_type n_rows, size_type n_cols)
        : std::vector<T>(n_rows * n_cols), rows_(n_rows), cols_(n_cols) {}

    Matrix(size_type n_rows, size_type n_cols, std::initializer_list<value_type> l)
        : rows_(n_rows), cols_(n_cols) {
        if (l.size() != rows_ * cols_) {
            throw std::invalid_argument("Incorrect initializer list size");
        }
        this->assign(l);
    }

    Matrix(const Matrix&) = default;
    Matrix& operator=(const Matrix&) = default;

    Matrix(Matrix&& rhs) noexcept
        : base(std::move(rhs)),
          rows_(std::exchange(rhs.rows_, 0)),
          cols_(std::exchange(rhs.cols_, 0)) {}

    Matrix& operator=(Matrix&& rhs) noexcept {
        base::operator=(std::move(rhs));
        rows_ = std::exchange(rhs.rows_, 0);
        cols_ = std::exchange(rhs.cols_, 0);
        return *this;
    }

    ~Matrix() = default;

    // 1. this is at(), not operator[]
    // 2. throw std::out_of_range
    // 3. it's better to start exception message with a small letter
    constexpr value_type& operator[](size_type row, size_type col) {
        if (!(row < rows_ && col < cols_)) {
            throw std::invalid_argument("Invalid matrix access");
        }
        return data()[row * cols_ + col];
    }

    constexpr const value_type& operator[](size_type row, size_type col) const {
        if (!(row < rows_ && col < cols_)) {
            throw std::invalid_argument("Invalid matrix access");
        }
        return data()[row * cols_ + row];
    }

    //-------------------------------
    size_type n_cols() const noexcept { return cols_; }
    size_type n_rows() const noexcept { return rows_; }
    using base::size;

    using base::begin;
    using base::cbegin;
    using base::cend;
    using base::crbegin;
    using base::crend;
    using base::end;
    using base::rbegin;
    using base::rend;

    bool operator==(const Matrix& rhs) const = default;
};

template <typename T>
Matrix<T> multiply(Matrix<T>& lhs, Matrix<T>& rhs) {
    const auto lhs_cols = lhs.n_cols();
    if (lhs_cols != rhs.n_rows()) {
        throw std::invalid_argument("Invalid matrix size");
    }

    const auto rhs_cols = rhs.n_cols();
    const auto lhs_rows = lhs.n_rows();
    
    Matrix<T> mul_matrix(lhs_rows, rhs_cols);

    for (std::size_t i = 0; i < lhs_rows; ++i) {
        for (std::size_t j = 0; j < rhs_cols; ++j) {
            for (std::size_t k = 0; k < lhs_cols; ++k) {
                mul_matrix[i, j] += lhs[i, k] * rhs[k, j];
            }
        }
    }

    return mul_matrix;
}
