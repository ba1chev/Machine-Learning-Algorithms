#pragma once

#include "source/learning_models/metrics/metric.hpp"
#include "source/learning_models/metrics/classification/utils.hpp"

template <class T>
class F1Score: public Metric<T> {
public:
    T compute(const Matrix<T>& y_true, const Matrix<T>& y_pred) const override;
    std::string get_name() const override;
    MetricType get_type() const override;
};

template <class T>
T F1Score<T>::compute(const Matrix<T>& y_true, const Matrix<T>& y_pred) const {
    this->validate_shapes(y_true, y_pred);

    ConfusionCounts<T> counts = CLASSIFICATION_DETAIL::count_confusion(y_true, y_pred);
    size_t predicted_positive = counts.true_positive + counts.false_positive;
    size_t actual_positive = counts.true_positive + counts.false_negative;

    if (predicted_positive == 0 || actual_positive == 0) {
        return T(0);
    }

    T precision = T(counts.true_positive) / T(predicted_positive);
    T recall = T(counts.true_positive) / T(actual_positive);

    if (precision + recall == T(0)) {
        return T(0);
    }

    return T(2) * (precision * recall) / (precision + recall);
}

template <class T>
std::string F1Score<T>::get_name() const {
    return "F1";
}

template <class T>
MetricType F1Score<T>::get_type() const {
    return MetricType::F1_SCORE;
}
