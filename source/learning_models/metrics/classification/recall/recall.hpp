#pragma once

#include "source/learning_models/metrics/metric.hpp"
#include "source/learning_models/metrics/classification/utils.hpp"

template <class T>
class Recall: public Metric<T> {
public:
    T compute(const Matrix<T>& y_true, const Matrix<T>& y_pred) const override;
    std::string get_name() const override;
    MetricType get_type() const override;
};

template <class T>
T Recall<T>::compute(const Matrix<T>& y_true, const Matrix<T>& y_pred) const {
    this->validate_shapes(y_true, y_pred);

    ConfusionCounts<T> counts = CLASSIFICATION_DETAIL::count_confusion(y_true, y_pred);
    size_t actual_positive = counts.true_positive + counts.false_negative;
    if (actual_positive == 0) {
        return T(0);
    }

    return T(counts.true_positive) / T(actual_positive);
}

template <class T>
std::string Recall<T>::get_name() const {
    return "Recall";
}

template <class T>
MetricType Recall<T>::get_type() const {
    return MetricType::RECALL;
}
