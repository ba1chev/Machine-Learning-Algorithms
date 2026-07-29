#pragma once

#include <cmath>
#include <memory>
#include <vector>
#include <limits>
#include <stdexcept>

#include "source/learning_models/learning_model_type.h"
#include "source/learning_models/unsupervised_learning/unsupervised_learning_model.hpp"

template <class T>
class KMeans: public UnSupervisedLearningModel<T> {
private:
    size_t k;
    size_t max_iters;

    size_t nearest_centroid(const Matrix<T>& X, size_t row, const Matrix<T>& centroids) const;

public:
    KMeans(const Matrix<T>* X, size_t k, size_t max_iters = 100);

    void fit() override;
    Matrix<T> predict(const Matrix<T>& X) const override;
    LearningModelType get_type() const override;
    std::unique_ptr<LearningModel<T>> clone() const override;
};

template <class T>
KMeans<T>::KMeans(const Matrix<T>* X, size_t k, size_t max_iters):
    UnSupervisedLearningModel<T>(X), k(k), max_iters(max_iters) {
    if (k == 0) {
        throw std::runtime_error("KMeans requires k >= 1");
    }
}

template <class T>
size_t KMeans<T>::nearest_centroid(const Matrix<T>& X, size_t row, const Matrix<T>& centroids) const {
    size_t features = X.get_cols_count();
    size_t best = 0;
    double best_distance = std::numeric_limits<double>::max();

    for (size_t c = 0; c < centroids.get_rows_count(); c++) {
        double distance = 0;

        for (size_t j = 0; j < features; j++) {
            double diff = (double)X[row][j] - (double)centroids[c][j];
            distance += diff * diff;
        }
        if (distance < best_distance) {
            best_distance = distance;
            best = c;
        }
    }

    return best;
}

template <class T>
void KMeans<T>::fit() {
    const Matrix<T>& X = *this->get_X();
    size_t n = X.get_rows_count();
    size_t features = X.get_cols_count();

    if (n < this->k) {
        throw std::runtime_error("KMeans: fewer samples than clusters");
    }

    Matrix<T> centroids;
    centroids.resize_dims(this->k, features);
    for (size_t c = 0; c < this->k; c++) {
        size_t seed = (c * n) / this->k;

        for (size_t j = 0; j < features; j++) {
            centroids[c][j] = X[seed][j];
        }
    }

    std::vector<size_t> assignments(n, 0);
    for (size_t iter = 0; iter < this->max_iters; iter++) {
        bool changed = false;

        for (size_t i = 0; i < n; i++) {
            size_t nearest = this->nearest_centroid(X, i, centroids);

            if (nearest != assignments[i]) {
                assignments[i] = nearest;
                changed = true;
            }
        }

        Matrix<T> sums;
        sums.resize_dims(this->k, features);
        std::vector<size_t> counts(this->k, 0);
        for (size_t i = 0; i < n; i++) {
            size_t c = assignments[i];
            counts[c] += 1;

            for (size_t j = 0; j < features; j++) {
                sums[c][j] = sums[c][j] + X[i][j];
            }
        }

        for (size_t c = 0; c < this->k; c++) {
            if (counts[c] > 0) {
                for (size_t j = 0; j < features; j++) {
                    centroids[c][j] = sums[c][j] / (T)counts[c];
                }
            }
        }

        if (!changed && iter > 0) {
            break;
        }
    }

    this->weights = centroids;
}

template <class T>
Matrix<T> KMeans<T>::predict(const Matrix<T>& X) const {
    if (this->weights.get_rows_count() == 0) {
        throw std::runtime_error("KMeans: model not fitted");
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
LearningModelType KMeans<T>::get_type() const {
    return LearningModelType::KMeans;
}

template <class T>
std::unique_ptr<LearningModel<T>> KMeans<T>::clone() const {
    return std::make_unique<KMeans<T>>(*this);
}
