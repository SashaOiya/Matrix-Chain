#pragma once

#include <cstddef>
#include <iterator>
#include <stdexcept>
#include <vector>

template <typename T>
class Matrix final: private std::vector<T>{
   private:
    std::size_t rows_;
    std::size_t cols_;

    template <typename data_type>
    struct Proxy_Row {
        data_type* row;
        data_type& operator[](std::size_t j) const { return row[j]; }
    };

    std::size_t index_ = 0;

   public:
    using value_type = T;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using iterator = typename std::vector<value_type>::iterator;
    using const_iterator = typename std::vector<value_type>::const_iterator;
    using reverse_iterator = typename std::vector<value_type>::reverse_iterator;
    using const_reverse_iterator = typename std::vector<value_type>::const_reverse_iterator;
    using iterator_category = typename std::iterator_traits<iterator>::iterator_category;

    using std::vector<value_type>::data;

    Matrix(size_type n_rows, size_type n_cols)
        : std::vector<T>(n_rows * n_cols), rows_(n_rows), cols_(n_cols){}

    Matrix(size_type n_rows, size_type n_cols, std::initializer_list<value_type> l)
        : std::vector<T>(l), rows_(n_rows), cols_(n_cols) {
        if (l.size() != size()) {
            throw std::invalid_argument("Incorrect initializer list size");
        }
    }

    Proxy_Row<value_type> operator[](size_type i) {
        if (rows_ <= i) throw std::invalid_argument("Rows out of bounds");
        return Proxy_Row<T>{data() + cols_ * i};
    }

    Proxy_Row<const value_type> operator[](size_type i) const {
        if (rows_ <= i) throw std::invalid_argument("Rows out of bounds");
        return Proxy_Row<const T>{data() + cols_ * i};
    }

    //-------------------------------
    size_type n_cols() const noexcept { return cols_; }
    size_type n_rows() const noexcept { return rows_; }
    using std::vector<value_type>::size;

    using std::vector<value_type>::begin;
    using std::vector<value_type>::end;
    using std::vector<value_type>::cbegin;
    using std::vector<value_type>::cend;
    using std::vector<value_type>::rbegin;
    using std::vector<value_type>::rend;
    using std::vector<value_type>::crbegin;
    using std::vector<value_type>::crend;

    bool equal(const Matrix& other) const { return data() == other.data(); }
};

template <typename T>
Matrix<T> multiply(Matrix<T>& A, Matrix<T>& rhs) {
    const std::size_t rhs_cols = rhs.n_cols();
    const std::size_t rhs_rows = rhs.n_rows();
    const std::size_t cols = A.n_cols();
    const std::size_t rows = A.n_rows();

    if (cols != rhs_rows) {
        throw std::invalid_argument("Invalid matrix size");
    }

    Matrix<T> mul_matrix(rows, rhs_cols);

    for (std::size_t i = 0; i < rows; ++i) {
        for (std::size_t j = 0; j < rhs_cols; ++j) {
            for (std::size_t k = 0; k < cols; ++k) {
                mul_matrix[i][j] += A[i][k] * rhs[k][j];
            }
        }
    }

    return mul_matrix;
}
