#pragma once

#include <cmath>
#include <memory>
#include <vector>
#include <limits>
#include <stdexcept>

#include "source/learning_models/learning_model_type.h"
#include "source/learning_models/unsupervised_learning/unsupervised_learning_model.hpp"

template <class T>
class HierarchicalClustering: public UnSupervisedLearningModel<T> {
private:
    size_t k;

    double average_linkage(const Matrix<T>& X, const std::vector<size_t>& a,
        const std::vector<size_t>& b) const;
    size_t nearest_centroid(const Matrix<T>& X, size_t row, const Matrix<T>& centroids) const;

public:
    HierarchicalClustering(const Matrix<T>* X, size_t k);

    void fit() override;
    Matrix<T> predict(const Matrix<T>& X) const override;
    LearningModelType get_type() const override;
    std::unique_ptr<LearningModel<T>> clone() const override;
};

template <class T>
HierarchicalClustering<T>::HierarchicalClustering(const Matrix<T>* X, size_t k):
    UnSupervisedLearningModel<T>(X), k(k) {
    if (k == 0) {
        throw std::runtime_error("HierarchicalClustering requires k >= 1");
    }
}

template <class T>
double HierarchicalClustering<T>::average_linkage(const Matrix<T>& X,
    const std::vector<size_t>& a, const std::vector<size_t>& b) const {
    double total = 0;
    for (size_t i : a) {
        for (size_t j : b) {
            double d = 0;
            for (size_t f = 0; f < X.get_cols_count(); f++) {
                double diff = (double)X[i][f] - (double)X[j][f];
                d += diff * diff;
            }
            total += std::sqrt(d);
        }
    }
    return total / (double)(a.size() * b.size());
}

template <class T>
size_t HierarchicalClustering<T>::nearest_centroid(const Matrix<T>& X, size_t row,
    const Matrix<T>& centroids) const {
    size_t best = 0;
    double best_distance = std::numeric_limits<double>::max();
    for (size_t c = 0; c < centroids.get_rows_count(); c++) {
        double d = 0;
        for (size_t j = 0; j < X.get_cols_count(); j++) {
            double diff = (double)X[row][j] - (double)centroids[c][j];
            d += diff * diff;
        }
        if (d < best_distance) {
            best_distance = d;
            best = c;
        }
    }
    return best;
}

template <class T>
void HierarchicalClustering<T>::fit() {
    const Matrix<T>& X = *this->get_X();
    size_t n = X.get_rows_count();
    size_t features = X.get_cols_count();

    if (n < this->k) {
        throw std::runtime_error("HierarchicalClustering: fewer samples than clusters");
    }

    std::vector<std::vector<size_t>> clusters;
    for (size_t i = 0; i < n; i++) {
        clusters.push_back({ i });
    }

    while (clusters.size() > this->k) {
        double best_distance = std::numeric_limits<double>::max();
        size_t merge_a = 0;
        size_t merge_b = 1;

        for (size_t a = 0; a < clusters.size(); a++) {
            for (size_t b = a + 1; b < clusters.size(); b++) {
                double d = this->average_linkage(X, clusters[a], clusters[b]);
                if (d < best_distance) {
                    best_distance = d;
                    merge_a = a;
                    merge_b = b;
                }
            }
        }

        for (size_t index : clusters[merge_b]) {
            clusters[merge_a].push_back(index);
        }
        clusters.erase(clusters.begin() + merge_b);
    }

    Matrix<T> centroids;
    centroids.resize_dims(clusters.size(), features);
    for (size_t c = 0; c < clusters.size(); c++) {
        for (size_t index : clusters[c]) {
            for (size_t j = 0; j < features; j++) {
                centroids[c][j] = centroids[c][j] + X[index][j];
            }
        }
        for (size_t j = 0; j < features; j++) {
            centroids[c][j] = centroids[c][j] / (T)clusters[c].size();
        }
    }

    this->weights = centroids;
}

template <class T>
Matrix<T> HierarchicalClustering<T>::predict(const Matrix<T>& X) const {
    if (this->weights.get_rows_count() == 0) {
        throw std::runtime_error("HierarchicalClustering: model not fitted");
    }
    if (X.get_cols_count() != this->weights.get_cols_count()) {
        throw std::runtime_error("Feature count does not match trained centroids");
    }

    size_t n = X.get_rows_count();
    Matrix<T> labels;
    labels.resize_dims(n, 1);
    for (size_t i = 0; i < n; i++) {
        labels[i][0] = (T)this->nearest_centroid(X, i, this->weights);
    }

    return labels;
}

template <class T>
LearningModelType HierarchicalClustering<T>::get_type() const {
    return LearningModelType::HierarchicalClustering;
}

template <class T>
std::unique_ptr<LearningModel<T>> HierarchicalClustering<T>::clone() const {
    return std::make_unique<HierarchicalClustering<T>>(*this);
}
