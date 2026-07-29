#pragma once

#include "source/learning_models/metrics/metric.hpp"
#include "source/learning_models/metrics/classification/utils.hpp"

template <class T>
class Precision: public Metric<T> {
public:
    T compute(const Matrix<T>& y_true, const Matrix<T>& y_pred) const override;
    std::string get_name() const override;
    MetricType get_type() const override;
};

template <class T>
T Precision<T>::compute(const Matrix<T>& y_true, const Matrix<T>& y_pred) const {
    this->validate_shapes(y_true, y_pred);

    ConfusionCounts<T> counts = CLASSIFICATION_DETAIL::count_confusion(y_true, y_pred);
    size_t predicted_positive = counts.true_positive + counts.false_positive;
    if (predicted_positive == 0) {
        return T(0);
    }

    return T(counts.true_positive) / T(predicted_positive);
}

template <class T>
std::string Precision<T>::get_name() const {
    return "Precision";
}

template <class T>
MetricType Precision<T>::get_type() const {
    return MetricType::PRECISION;
}
