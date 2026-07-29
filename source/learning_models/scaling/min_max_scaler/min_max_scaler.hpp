#pragma once

#include <stdexcept>

#include "source/learning_models/scaling/scaler.hpp"

template <class T>
class MinMaxScaler: public Scaler<T> {
public:
    void fit(const Matrix<T>& data) override;
    Matrix<T> transform(const Matrix<T>& data) const override;
    ScalerType get_type() const override;
};

template <class T>
void MinMaxScaler<T>::fit(const Matrix<T>& data) {
    size_t rows = data.get_rows_count();
    size_t cols = data.get_cols_count();
    if (rows == 0 || cols == 0) {
        throw std::runtime_error("Cannot fit scaler on empty data");
    }

    this->parameters.resize_dims(2, cols);

    for (size_t j = 0; j < cols; j++) {
        T min = data[0][j];
        T max = data[0][j];
        for (size_t i = 1; i < rows; i++) {
            if (data[i][j] < min) {
                min = data[i][j];
            }
            if (data[i][j] > max) {
                max = data[i][j];
            }
        }

        this->parameters[0][j] = min;
        this->parameters[1][j] = max;
    }

    this->fitted = true;
}

template <class T>
Matrix<T> MinMaxScaler<T>::transform(const Matrix<T>& data) const {
    if (!this->fitted) {
        throw std::runtime_error("Scaler must be fitted before transform");
    }
    if (data.get_cols_count() != this->parameters.get_cols_count()) {
        throw std::runtime_error("Feature count does not match fitted scaler");
    }

    size_t rows = data.get_rows_count();
    size_t cols = data.get_cols_count();

    Matrix<T> result;
    result.resize_dims(rows, cols);

    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            T min = this->parameters[0][j];
            T max = this->parameters[1][j];
            T range = max - min;
            if (range == T(0)) {
                range = T(1);
            }
            result[i][j] = (data[i][j] - min) / range;
        }
    }

    return result;
}

template <class T>
ScalerType MinMaxScaler<T>::get_type() const {
    return ScalerType::MIN_MAX_SCALER;
}
