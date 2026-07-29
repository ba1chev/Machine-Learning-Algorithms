#pragma once

#include <cstddef>

struct DecisionNode {
public:
    size_t feature_index = 0;
    float threshold = 0.f;
    float value = 0.f;
    bool is_leaf = false;
};
