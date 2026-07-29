#pragma once

#include <cmath>
#include <stdexcept>

#include "source/learning_models/scaling/scaler.hpp"

template <class T>
class NormalScaler: public Scaler<T> {
public:
    void fit(const Matrix<T>& data) override;
    Matrix<T> transform(const Matrix<T>& data) const override;
    ScalerType get_type() const override;
};

template <class T>
void NormalScaler<T>::fit(const Matrix<T>& data) {
    size_t rows = data.get_rows_count();
    size_t cols = data.get_cols_count();
    if (rows == 0 || cols == 0) {
        throw std::runtime_error("Cannot fit scaler on empty data");
    }

    this->parameters.resize_dims(2, cols);

    for (size_t j = 0; j < cols; j++) {
        T sum = T(0);
        for (size_t i = 0; i < rows; i++) {
            sum += data[i][j];
        }
        T mean = sum / T(rows);

        T variance = T(0);
        for (size_t i = 0; i < rows; i++) {
            T diff = data[i][j] - mean;
            variance += diff * diff;
        }
        T std = std::sqrt(variance / T(rows));
        if (std == T(0)) {
            std = T(1);
        }

        this->parameters[0][j] = mean;
        this->parameters[1][j] = std;
    }

    this->fitted = true;
}

template <class T>
Matrix<T> NormalScaler<T>::transform(const Matrix<T>& data) const {
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
            T mean = this->parameters[0][j];
            T std = this->parameters[1][j];
            result[i][j] = (data[i][j] - mean) / std;
        }
    }

    return result;
}

template <class T>
ScalerType NormalScaler<T>::get_type() const {
    return ScalerType::NORMAL_SCALER;
}
