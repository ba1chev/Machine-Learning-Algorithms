#pragma once

#include <cstddef>

template <class T>
struct ConfusionCounts {
    size_t true_positive = 0;
    size_t false_positive = 0;
    size_t false_negative = 0;
    size_t true_negative = 0;
};