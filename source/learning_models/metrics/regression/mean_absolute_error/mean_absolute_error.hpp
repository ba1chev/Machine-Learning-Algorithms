#pragma once

#include <cmath>

#include "source/learning_models/metrics/metric.hpp"

template <class T>
class MeanAbsoluteError: public Metric<T> {
public:
    T compute(const Matrix<T>& y_true, const Matrix<T>& y_pred) const override;
    std::string get_name() const override;
    MetricType get_type() const override;
};

template <class T>
T MeanAbsoluteError<T>::compute(const Matrix<T>& y_true, const Matrix<T>& y_pred) const {
    this->validate_shapes(y_true, y_pred);

    size_t rows = y_true.get_rows_count();
    T sum = T(0);
    for (size_t i = 0; i < rows; i++) {
        sum += std::abs(y_true[i][0] - y_pred[i][0]);
    }

    return sum / T(rows);
}

template <class T>
std::string MeanAbsoluteError<T>::get_name() const {
    return "MAE";
}

template <class T>
MetricType MeanAbsoluteError<T>::get_type() const {
    return MetricType::MEAN_ABSOLUTE_ERROR;
}
