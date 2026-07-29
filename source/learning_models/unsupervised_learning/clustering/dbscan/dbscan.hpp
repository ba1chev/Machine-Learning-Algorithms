#pragma once

#include <cmath>
#include <memory>
#include <vector>
#include <limits>
#include <stdexcept>

#include "source/learning_models/learning_model.hpp"
#include "source/learning_models/learning_model_type.h"
#include "source/learning_models/unsupervised_learning/unsupervised_learning_model.hpp"

template <class T>
class DBSCAN: public UnSupervisedLearningModel<T> {
private:
    static constexpr int UNCLASSIFIED = -1;
    static constexpr int NOISE = -2;

    double eps;
    size_t min_samples;

    double distance(const Matrix<T>& X, size_t a, size_t b) const;
    std::vector<size_t> region_query(const Matrix<T>& X, size_t point) const;
    size_t nearest_centroid(const Matrix<T>& X, size_t row, const Matrix<T>& centroids) const;

public:
    DBSCAN(const Matrix<T>* X, double eps, size_t min_samples);

    void fit() override;
    Matrix<T> predict(const Matrix<T>& X) const override;
    LearningModelType get_type() const override;
    std::unique_ptr<LearningModel<T>> clone() const override;
};

template <class T>
DBSCAN<T>::DBSCAN(const Matrix<T>* X, double eps, size_t min_samples):
    UnSupervisedLearningModel<T>(X), eps(eps), min_samples(min_samples) {
    if (eps <= 0) {
        throw std::runtime_error("DBSCAN requires eps > 0");
    }
}

template <class T>
double DBSCAN<T>::distance(const Matrix<T>& X, size_t a, size_t b) const {
    double sum = 0;
    for (size_t j = 0; j < X.get_cols_count(); j++) {
        double diff = (double)X[a][j] - (double)X[b][j];
        sum += diff * diff;
    }
    return std::sqrt(sum);
}

template <class T>
std::vector<size_t> DBSCAN<T>::region_query(const Matrix<T>& X, size_t point) const {
    std::vector<size_t> neighbours;
    for (size_t i = 0; i < X.get_rows_count(); i++) {
        if (this->distance(X, point, i) <= this->eps) {
            neighbours.push_back(i);
        }
    }
    return neighbours;
}

template <class T>
size_t DBSCAN<T>::nearest_centroid(const Matrix<T>& X, size_t row, const Matrix<T>& centroids) const {
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
void DBSCAN<T>::fit() {
    const Matrix<T>& X = *this->get_X();
    size_t n = X.get_rows_count();
    size_t features = X.get_cols_count();

    std::vector<int> labels(n, UNCLASSIFIED);
    int cluster_id = 0;

    for (size_t i = 0; i < n; i++) {
        if (labels[i] != UNCLASSIFIED) {
            continue;
        }

        std::vector<size_t> neighbours = this->region_query(X, i);
        if (neighbours.size() < this->min_samples) {
            labels[i] = NOISE;
            continue;
        }

        labels[i] = cluster_id;
        std::vector<size_t> seeds = neighbours;
        for (size_t s = 0; s < seeds.size(); s++) {
            size_t current = seeds[s];
            if (labels[current] == NOISE) {
                labels[current] = cluster_id;
            }
            if (labels[current] != UNCLASSIFIED) {
                continue;
            }

            labels[current] = cluster_id;
            std::vector<size_t> current_neighbours = this->region_query(X, current);
            if (current_neighbours.size() >= this->min_samples) {
                for (size_t neighbour : current_neighbours) {
                    seeds.push_back(neighbour);
                }
            }
        }

        cluster_id++;
    }

    Matrix<T> centroids;
    centroids.resize_dims((size_t)cluster_id, features);
    std::vector<size_t> counts((size_t)cluster_id, 0);
    for (size_t i = 0; i < n; i++) {
        if (labels[i] >= 0) {
            size_t c = (size_t)labels[i];
            counts[c]++;
            for (size_t j = 0; j < features; j++) {
                centroids[c][j] = centroids[c][j] + X[i][j];
            }
        }
    }
    for (size_t c = 0; c < (size_t)cluster_id; c++) {
        if (counts[c] > 0) {
            for (size_t j = 0; j < features; j++) {
                centroids[c][j] = centroids[c][j] / (T)counts[c];
            }
        }
    }

    this->weights = centroids;
}

template <class T>
Matrix<T> DBSCAN<T>::predict(const Matrix<T>& X) const {
    if (this->weights.get_rows_count() == 0) {
        throw std::runtime_error("DBSCAN: model produced no clusters (all noise) or not fitted");
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
LearningModelType DBSCAN<T>::get_type() const {
    return LearningModelType::DBSCAN;
}

template <class T>
std::unique_ptr<LearningModel<T>> DBSCAN<T>::clone() const {
    return std::make_unique<DBSCAN<T>>(*this);
}
