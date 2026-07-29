#pragma once

#include <string>
#include <stdexcept>

#include "source/learning_models/metrics/metric_type.h"
#include "source/data/data_structures/matrix/matrix.hpp"

template <class T>
class Metric {
protected:
    void validate_shapes(const Matrix<T>& y_true, const Matrix<T>& y_pred) const;

public:
    virtual T compute(const Matrix<T>& y_true, const Matrix<T>& y_pred) const = 0;

    virtual std::string get_name() const = 0;
    virtual MetricType get_type() const = 0;
    virtual ~Metric() = default;
};

template <class T>
void Metric<T>::validate_shapes(const Matrix<T>& y_true, const Matrix<T>& y_pred) const {
    if (y_true.get_rows_count() != y_pred.get_rows_count()) {
        throw std::runtime_error("Metric: y_true and y_pred row count mismatch");
    }
    if (y_true.get_rows_count() == 0) {
        throw std::runtime_error("Metric: cannot compute on empty data");
    }
}
