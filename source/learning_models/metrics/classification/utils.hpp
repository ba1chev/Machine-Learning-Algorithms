#pragma once

#include <cstddef>

#include "source/data/data_structures/matrix/matrix.hpp"
#include "source/learning_models/metrics/classification/confusion_counts.hpp"

namespace CLASSIFICATION_DETAIL {
    template <class T>
    ConfusionCounts<T> count_confusion(const Matrix<T>& y_true, const Matrix<T>& y_pred);
}

template <class T>
ConfusionCounts<T> CLASSIFICATION_DETAIL::count_confusion(const Matrix<T>& y_true, const Matrix<T>& y_pred) {
    ConfusionCounts<T> counts;
    size_t rows = y_true.get_rows_count();
    for (size_t i = 0; i < rows; i++) {
        bool actual = y_true[i][0] >= T(1) / T(2);
        bool predicted = y_pred[i][0] >= T(1) / T(2);
        if (actual && predicted) {
            counts.true_positive++;
        } else if (!actual && predicted) {
            counts.false_positive++;
        } else if (actual && !predicted) {
            counts.false_negative++;
        } else {
            counts.true_negative++;
        }
    }
    return counts;
}