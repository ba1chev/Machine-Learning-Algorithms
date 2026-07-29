#pragma once

#include "source/learning_models/metrics/metric.hpp"

template <class T>
class RSquared: public Metric<T> {
public:
    T compute(const Matrix<T>& y_true, const Matrix<T>& y_pred) const override;
    std::string get_name() const override;
    MetricType get_type() const override;
};

template <class T>
T RSquared<T>::compute(const Matrix<T>& y_true, const Matrix<T>& y_pred) const {
    this->validate_shapes(y_true, y_pred);

    size_t rows = y_true.get_rows_count();

    T mean = T(0);
    for (size_t i = 0; i < rows; i++) {
        mean += y_true[i][0];
    }
    mean /= T(rows);

    T residual_sum = T(0);
    T total_sum = T(0);
    for (size_t i = 0; i < rows; i++) {
        T residual = y_true[i][0] - y_pred[i][0];
        T deviation = y_true[i][0] - mean;
        residual_sum += residual * residual;
        total_sum += deviation * deviation;
    }

    if (total_sum == T(0)) {
        return T(0);
    }

    return T(1) - (residual_sum / total_sum);
}

template <class T>
std::string RSquared<T>::get_name() const {
    return "R2";
}

template <class T>
MetricType RSquared<T>::get_type() const {
    return MetricType::R_SQUARED;
}
