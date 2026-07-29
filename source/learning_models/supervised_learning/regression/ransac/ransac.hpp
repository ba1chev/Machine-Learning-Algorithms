#pragma once

#include <cmath>
#include <random>
#include <memory>
#include <stdexcept>

#include "source/learning_models/learning_model_type.h"
#include "source/learning_models/supervised_learning/supervised_learning_model.hpp"
#include "source/learning_models/supervised_learning/regression/linear_regression/linear_regression.hpp"

template <class T>
class RANSAC: public SupervisedLearningModel<T> {
private:
    size_t iterations;
    size_t sample_size;
    T inlier_threshold;

    size_t count_inliers(const Matrix<T>& weights) const;

public:
    RANSAC(const Matrix<T>* X, const Matrix<T>* y, size_t iterations, size_t sample_size, T inlier_threshold);

    void fit() override;
    Matrix<T> predict(const Matrix<T>& X) const override;
    LearningModelType get_type() const override;
    std::unique_ptr<LearningModel<T>> clone() const override;
};

template <class T>
RANSAC<T>::RANSAC(const Matrix<T>* X, const Matrix<T>* y, size_t iterations, size_t sample_size, T inlier_threshold)
    : SupervisedLearningModel<T>(X, y), iterations(iterations), sample_size(sample_size), inlier_threshold(inlier_threshold) {
    if (sample_size == 0 || sample_size > X->get_rows_count()) {
        throw std::runtime_error("Invalid RANSAC sample size");
    }
}

template <class T>
size_t RANSAC<T>::count_inliers(const Matrix<T>& weights) const {
    Matrix<T> predictions = (*this->X) * weights;
    size_t count = 0;

    for (size_t i = 0; i < predictions.get_rows_count(); i++) {
        T error = predictions[i][0] - (*this->y)[i][0];
        if (std::abs(error) <= this->inlier_threshold) {
            count += 1;
        }
    }

    return count;
}

template <class T>
void RANSAC<T>::fit() {
    size_t total_rows = this->X->get_rows_count();
    size_t feature_count = this->X->get_cols_count();

    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<size_t> pick(0, total_rows - 1);

    Matrix<T> best_weights = this->weights;
    size_t best_inliers = 0;

    Matrix<T> sample_X, sample_y;
    sample_X.resize_dims(this->sample_size, feature_count);
    sample_y.resize_dims(this->sample_size, 1);

    for (size_t iteration = 0; iteration < this->iterations; iteration++) {
        for (size_t s = 0; s < this->sample_size; s++) {
            size_t row = pick(rng);
            for (size_t j = 0; j < feature_count; j++) {
                sample_X[s][j] = (*this->X)[row][j];
            }
            sample_y[s][0] = (*this->y)[row][0];
        }

        LinearRegression<T> candidate(&sample_X, &sample_y);
        if (this->get_optimizer()) {
            candidate.set_optimizer(this->get_optimizer()->clone());
        }
        candidate.fit();

        size_t inliers = count_inliers(candidate.get_weights());
        if (inliers > best_inliers) {
            best_inliers = inliers;
            best_weights = candidate.get_weights();
        }
    }

    this->set_weights(best_weights);
}

template <class T>
Matrix<T> RANSAC<T>::predict(const Matrix<T>& X) const {
    if (X.get_cols_count() != this->weights.get_rows_count()) {
        throw std::runtime_error("Feature count does not match trained weights");
    }

    return X * this->weights;
}

template <class T>
LearningModelType RANSAC<T>::get_type() const {
    return LearningModelType::RANSAC;
}

template <class T>
std::unique_ptr<LearningModel<T>> RANSAC<T>::clone() const {
    return std::make_unique<RANSAC<T>>(*this);
}
