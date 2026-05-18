#pragma once

#include "../vector/vector.hpp"

template <class T>
class Matrix {
private:
    Vector<Vector<T>> data;
    size_t countOfRows = 0;
    size_t countOfCols = 0;

public:
    Matrix(size_t countOfRows, size_t countOfCols);
    Matrix& operator *= (const Matrix& other);
    Matrix& operator *= (const float scalar);
    Matrix& operator += (const Matrix& other);
    Matrix& operator -= (const Matrix& other);

    Vector<T>& operator [] (size_t index);
    const Vector<T>& operator [] (size_t index) const;

    void pop_back();
    void push_back(Vector<T>&& row);
    void push_back(const Vector<T>& row);
    Matrix transpose() const;

    size_t getCountOfRows() const;
    size_t getCountOfCols() const;
    const Vector<Vector<T>>& getDataNonRef() const;
    Vector<Vector<T>>& getDataByRef();
};

template <class T>
Matrix<T> operator + (const Matrix<T>& left, const Matrix<T>& right) {
    Matrix<T> result(left);
    result += right;
    return result;
}

template <class T>
Matrix<T> operator - (const Matrix<T>& left, const Matrix<T>& right) {
    Matrix<T> result(left);
    result -= right;
    return result;
}

template <class T>
Matrix<T> operator * (const Matrix<T>& left, const Matrix<T>& right) {
    Matrix<T> result(left);
    result *= right;
    return result;
}

template <class T>
Matrix<T> operator * (const Matrix<T>& left, const float scalar) {
    Matrix<T> result(left);
    result *= scalar;
    return result;
}

template <class T>
Matrix<T> operator * (const float scalar, const Matrix<T>& right) {
    Matrix<T> result(right);
    result *= scalar;
    return result;
}

template <class T>
Matrix<T>::Matrix(size_t countOfRows, size_t countOfCols) {
    this->countOfRows = countOfRows;
    this->countOfCols = countOfCols;
    this->data = Vector<Vector<T>>();

    for (size_t i = 0; i < countOfRows; i++) {
        Vector<T> row;
        for (size_t j = 0; j < countOfCols; j++) {
            row.push_back(T{});
        }
        this->data.push_back(std::move(row));
    }
}

template <class T>
Matrix<T>& Matrix<T>::operator *= (const Matrix& other) {
    if (this != &other) {
        if (this->countOfCols == other.countOfRows) {
            Matrix<T> newMatrix(this->countOfRows, other.countOfCols);

            for (size_t i = 0; i < this->countOfRows; i++) {
                for (size_t k = 0; k < other.countOfCols; k++) {
                    for (size_t j = 0; j < this->countOfCols; j++) {
                        newMatrix.data[i][k] += this->data[i][j] * other.data[j][k];
                    }
                }
            }
            *this = newMatrix;
        } else {
            throw std::runtime_error("The count of cols for the left matrix is not equl \
                to the count of rows of the right matrix");
        }
    }

    return *this;
}

template <class T>
Matrix<T>& Matrix<T>::operator *= (const float scalar) {
    for (size_t i = 0; i < this->countOfRows; i++) {
        for (size_t j = 0; j < this->countOfCols; j++) {
            this->data[i][j] *= scalar;            
        }
    }

    return *this;
}

template <class T>
Matrix<T>& Matrix<T>::operator += (const Matrix& other) {
    if (this->countOfRows != other.countOfRows || this->countOfCols != other.countOfCols) {
        throw std::runtime_error("The matrices are not from one dim");
    }

    for (size_t i = 0; i < this->countOfRows; i++) {
        for (size_t j = 0; j < this->countOfCols; j++) {
            this->data[i][j] += other.data[i][j];
        }
    }

    return *this;
}

template <class T>
Matrix<T>& Matrix<T>::operator -= (const Matrix& other) {
    if (this->countOfRows != other.countOfRows || this->countOfCols != other.countOfCols) {
        throw std::runtime_error("The matrices are not from one dim");
    }

    for (size_t i = 0; i < this->countOfRows; i++) {
        for (size_t j = 0; j < this->countOfCols; j++) {
            this->data[i][j] -= other.data[i][j];
        }
    }

    return *this;
}

template <class T>
void Matrix<T>::pop_back() {
    this->data.pop_back();
}

template <class T>
void Matrix<T>::push_back(Vector<T>&& row) {
    this->data.push_back(std::move(row));
}

template <class T>
void Matrix<T>::push_back(const Vector<T>& row) {
    this->data.push_back(row);
}

template <class T>
Matrix<T> Matrix<T>::transpose() const {
    Matrix<T> result(this->countOfCols, this->countOfRows);

    for (size_t i = 0; i < this->countOfRows; i++) {
        for (size_t j = 0; j < this->countOfCols; j++) {
            result.data[j][i] = this->data[i][j];
        }
    }

    return result;
}

template <class T>
size_t Matrix<T>::getCountOfRows() const {
    return this->countOfRows;
}

template <class T>
size_t Matrix<T>::getCountOfCols() const {
    return this->countOfCols;
}

template <class T>
const Vector<Vector<T>>& Matrix<T>::getDataNonRef() const {
    return this->data;
}

template <class T>
Vector<Vector<T>>& Matrix<T>::getDataByRef() {
    return this->data;
}

template <class T>
Vector<T>& Matrix<T>::operator [] (size_t index) {
    if (index >= this->countOfRows) {
        throw std::out_of_range("Index is out of range");
    }

    return this->data[index];
}

template <class T>
const Vector<T>& Matrix<T>::operator [] (size_t index) const {
    if (index >= this->countOfRows) {
        throw std::out_of_range("Index is out of range");
    }

    return this->data[index];
}