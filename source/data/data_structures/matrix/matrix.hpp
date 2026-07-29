#pragma once

#include "vector"
#include "iostream"

template <class T>
class Matrix {
private:
    size_t count_of_rows = 0;
    size_t count_of_cols = 0;
    std::vector<std::vector<T>> data;

public:
    Matrix() = default;
    Matrix(const std::vector<std::vector<T>>& data);
    Matrix(const T* const* data, size_t count_of_rows, size_t count_of_cols);
    Matrix& operator *= (const Matrix& other);
    Matrix& operator *= (const T& scalar);
    Matrix& operator += (const Matrix& other);
    Matrix& operator -= (const Matrix& other);

    static Matrix identity(size_t size);

    Matrix transpose() const;
    size_t get_rows_count() const;
    size_t get_cols_count() const;
    const std::vector<std::vector<T>>& get_data() const;

    const std::vector<T>& operator [] (size_t index) const;
    std::vector<T>& operator [] (size_t index);
    void resize_dims(size_t new_rows, size_t new_cols);

    template <class U>
    friend std::istream& operator >> (std::istream& is, Matrix<U>& matrix);

    template <class U>
    friend std::ostream& operator << (std::ostream& os, const Matrix<U>& matrix);
};

template<class T>
Matrix<T> operator * (const Matrix<T>& left, const Matrix<T>& right) {
    Matrix<T> result = left;
    result *= right;
    return result;
}

template<class T>
Matrix<T> operator * (const Matrix<T>& matrix, const T& scalar) {
    Matrix<T> result = matrix;
    result *= scalar;
    return result;
}

template<class T>
Matrix<T> operator * (const T& scalar, const Matrix<T>& matrix) {
    return matrix * scalar;
}

template<class T>
Matrix<T> operator + (const Matrix<T>& left, const Matrix<T>& right) {
    Matrix<T> result = left;
    result += right;
    return result;
}

template<class T>
Matrix<T> operator - (const Matrix<T>& left, const Matrix<T>& right) {
    Matrix<T> result = left;
    result -= right;
    return result;
}

template <class T>
Matrix<T>::Matrix(const T* const* data, size_t count_of_rows, size_t count_of_cols) {
    if (!data) {
        throw std::runtime_error("Nullptr detected");
    }

    for (size_t i = 0; i < count_of_rows; i++) {
        if (!data[i]) {
            throw std::runtime_error("Nullptr detected");
        }
    }
    
    this->count_of_rows = count_of_rows;
    this->count_of_cols = count_of_cols;
    this->data = std::vector<std::vector<T>>(
        count_of_rows, std::vector<T>(count_of_cols, T{})
    );

    for (size_t i = 0; i < this->count_of_rows; i++) {
        for (size_t j = 0; j < this->count_of_cols; j++) {
            this->data[i][j] = data[i][j];
        }
    }
}

template <class T>
Matrix<T>::Matrix(const std::vector<std::vector<T>>& data) {
    this->count_of_rows = data.size();
    this->count_of_cols = data.empty() ? 0 : data[0].size();
    this->data = data;
}
    
template <class T>
Matrix<T>& Matrix<T>::operator *= (const Matrix<T>& other) {
    if (this != &other) {
        if (this->count_of_cols != other.get_rows_count()) {
            throw std::runtime_error("Matrix dimensions do not match for multiplication");
        }

        size_t count_of_rows = this->count_of_rows;
        size_t count_of_cols = other.get_cols_count();
        std::vector<std::vector<T>> result_matrix = std::vector<std::vector<T>>(
            count_of_rows, std::vector<T>(count_of_cols, T{})
        );

        for (size_t i = 0; i < count_of_rows; i++) {
            for (size_t j = 0; j < count_of_cols; j++) {
                for (size_t k = 0; k < other.get_rows_count(); k++) {
                    result_matrix[i][j] += this->data[i][k] * other.get_data()[k][j];
                }
            }
        }

        this->count_of_rows = count_of_rows;
        this->count_of_cols = count_of_cols;
        this->data = result_matrix;
    }

    return *this;
}

template <class T>
Matrix<T>& Matrix<T>::operator *= (const T& scalar) {
    for (size_t i = 0; i < this->count_of_rows; i++) {
        for (size_t j = 0; j < this->count_of_cols; j++) {
            this->data[i][j] *= scalar;
        }
    }

    return *this;
}

template <class T>
Matrix<T> Matrix<T>::identity(size_t size) {
    Matrix<T> result;
    result.resize_dims(size, size);
    for (size_t i = 0; i < size; i++) {
        result[i][i] = T(1);
    }

    return result;
}

template <class T>
Matrix<T>& Matrix<T>::operator += (const Matrix<T>& other) {
    if (this != &other) {
        if (this->count_of_rows != other.get_rows_count() ||
            this->count_of_cols != other.get_cols_count()) {
                throw std::runtime_error("Matrix dimensions do not match for addition");
        }

        for (size_t i = 0; i < this->count_of_rows; i++) {
            for (size_t j = 0; j < this->count_of_cols; j++) {
                this->data[i][j] += other.get_data()[i][j];
            }
        }
    }

    return *this;
}
    
template <class T>
Matrix<T>& Matrix<T>::operator -= (const Matrix<T>& other) {
    if (this != &other) {
        if (this->count_of_rows != other.get_rows_count() ||
            this->count_of_cols != other.get_cols_count()) {
                throw std::runtime_error("Matrix dimensions do not match for subtraction");
        }

        for (size_t i = 0; i < this->count_of_rows; i++) {
            for (size_t j = 0; j < this->count_of_cols; j++) {
                this->data[i][j] -= other.get_data()[i][j];
            }
        }
    }

    return *this;
}

template <class T>
Matrix<T> Matrix<T>::transpose() const {
    std::vector<std::vector<T>> new_data(
        this->count_of_cols, std::vector<T>(this->count_of_rows, T{})
    );

    for (size_t i = 0; i < this->count_of_rows; i++) {
        for (size_t j = 0; j < this->count_of_cols; j++) {
            new_data[j][i] = this->data[i][j];
        }
    }

    Matrix<T> result(new_data);
    return result;
}

template <class T>
size_t Matrix<T>::get_rows_count() const {
    return this->count_of_rows;
}

template <class T>
size_t Matrix<T>::get_cols_count() const {
    return this->count_of_cols;
}

template <class T>
const std::vector<std::vector<T>>& Matrix<T>::get_data() const {
    return this->data;
}

template <class T>
const std::vector<T>& Matrix<T>::operator [] (size_t index) const {
    if (index >= this->count_of_rows) {
        throw std::out_of_range("Index is out of range");
    }

    return this->data[index];
}

template <class T>
void Matrix<T>::resize_dims(size_t new_rows, size_t new_cols) {
    this->count_of_rows = new_rows;
    this->count_of_cols = new_cols;
    this->data = std::vector<std::vector<T>>(
        this->count_of_rows, std::vector<T>(this->count_of_cols, T{})
    ); 
}

template <class T>
std::vector<T>& Matrix<T>::operator [] (size_t index) {
    if (index >= this->count_of_rows) {
        throw std::out_of_range("Index is out of range");
    }

    return this->data[index];
}

template <class T>
std::istream& operator >> (std::istream& is, Matrix<T>& matrix) {
    size_t count_of_rows = matrix.get_rows_count();
    size_t count_of_cols = matrix.get_cols_count();

    for (size_t i = 0; i < count_of_rows; i++) {
        for (size_t j = 0; j < count_of_cols; j++) {
            is >> matrix.data[i][j];
        }
    }

    return is;
}

template <class T>
std::ostream& operator << (std::ostream& os, const Matrix<T>& matrix) {
    size_t count_of_rows = matrix.get_rows_count();
    size_t count_of_cols = matrix.get_cols_count();

    os << "[ ";
    for (size_t i = 0; i < count_of_rows; i++) {
        for (size_t j = 0; j < count_of_cols; j++) {
            os << matrix.get_data()[i][j];
        }
        os << std::endl;
    }
    os << "]";

    return os;
}