#pragma once

#include <cmath>

#include "source/learning_models/metrics/metric.hpp"

template <class T>
class RootMeanSquaredError: public Metric<T> {
public:
    T compute(const Matrix<T>& y_true, const Matrix<T>& y_pred) const override;
    std::string get_name() const override;
    MetricType get_type() const override;
};

template <class T>
T RootMeanSquaredError<T>::compute(const Matrix<T>& y_true, const Matrix<T>& y_pred) const {
    this->validate_shapes(y_true, y_pred);

    size_t rows = y_true.get_rows_count();
    T sum = T(0);
    for (size_t i = 0; i < rows; i++) {
        T diff = y_true[i][0] - y_pred[i][0];
        sum += diff * diff;
    }

    return std::sqrt(sum / T(rows));
}

template <class T>
std::string RootMeanSquaredError<T>::get_name() const {
    return "RMSE";
}

template <class T>
MetricType RootMeanSquaredError<T>::get_type() const {
    return MetricType::ROOT_MEAN_SQUARED_ERROR;
}
