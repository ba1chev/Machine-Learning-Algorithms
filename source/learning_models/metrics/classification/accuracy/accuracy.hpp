#pragma once

#include "source/learning_models/metrics/metric.hpp"

template <class T>
class Accuracy: public Metric<T> {
public:
    T compute(const Matrix<T>& y_true, const Matrix<T>& y_pred) const override;
    std::string get_name() const override;
    MetricType get_type() const override;
};

template <class T>
T Accuracy<T>::compute(const Matrix<T>& y_true, const Matrix<T>& y_pred) const {
    this->validate_shapes(y_true, y_pred);

    size_t rows = y_true.get_rows_count();
    size_t correct = 0;
    for (size_t i = 0; i < rows; i++) {
        bool actual = y_true[i][0] >= T(1) / T(2);
        bool predicted = y_pred[i][0] >= T(1) / T(2);
        if (actual == predicted) {
            correct++;
        }
    }

    return T(correct) / T(rows);
}

template <class T>
std::string Accuracy<T>::get_name() const {
    return "Accuracy";
}

template <class T>
MetricType Accuracy<T>::get_type() const {
    return MetricType::ACCURACY;
}
