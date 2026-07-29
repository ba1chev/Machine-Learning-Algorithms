#pragma once

enum class MetricType {
    // Regression
    MEAN_SQUARED_ERROR = (1 << 0),
    ROOT_MEAN_SQUARED_ERROR = (1 << 1),
    MEAN_ABSOLUTE_ERROR = (1 << 2),
    R_SQUARED = (1 << 3),

    // Classification
    ACCURACY = (1 << 4),
    PRECISION = (1 << 5),
    RECALL = (1 << 6),
    F1_SCORE = (1 << 7)
};
