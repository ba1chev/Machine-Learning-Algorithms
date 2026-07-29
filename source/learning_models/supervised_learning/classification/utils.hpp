#pragma once

#include <map>
#include <vector>

#include "source/data/data_structures/matrix/matrix.hpp"

namespace DECISION_TREE {
    template<class T>
    float gini(const std::vector<size_t>& rows, const Matrix<T>& y);

    template<class T>
    float majority_class(const std::vector<size_t>& rows, const Matrix<T>& y);

    template<class T>
    bool is_pure(const std::vector<size_t>& rows, const Matrix<T>& y);
};

template <class T>
float DECISION_TREE::gini(const std::vector<size_t>& rows, const Matrix<T>& y) {
    if (rows.empty()) return 0.f;
    std::map<float, size_t> counts;
    
    for (size_t i : rows) {
        counts[(float)y[i][0]] += 1;
    }

    float total = (float)rows.size();
    float impurity = 1.f;

    for (const auto& class_count : counts) {
        float p = class_count.second / total;
        impurity -= p * p;
    }

    return impurity;
}

template <class T>
float DECISION_TREE::majority_class(const std::vector<size_t>& rows, const Matrix<T>& y) {
    std::map<float, size_t> counts;
    for (size_t i : rows) {
        counts[(float)y[i][0]]++;
    }

    float best_label = 0.f;
    size_t best_count = 0;

    for (const auto& class_count : counts) {
        if (class_count.second > best_count) {
            best_count = class_count.second;
            best_label = class_count.first;
        }
    }

    return best_label;
}

template <class T>
bool DECISION_TREE::is_pure(const std::vector<size_t>& rows, const Matrix<T>& y) {
    if (rows.empty()) return true;
    float first = (float)y[rows[0]][0];

    for (size_t i : rows) {
        if ((float)y[i][0] != first) return false;
    }

    return true;
}