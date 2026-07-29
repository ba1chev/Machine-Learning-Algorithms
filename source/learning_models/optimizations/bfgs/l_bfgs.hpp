#pragma once

#include <cmath>
#include <deque>
#include <memory>
#include <vector>
#include <stdexcept>

#include "source/learning_models/learning_model_type.h"
#include "source/learning_models/optimizations/optimizator.hpp"
#include "source/learning_models/optimizations/optimizator_type.h"
#include "source/learning_models/supervised_learning/supervised_learning_model.hpp"

template <class T>
class LBFGSOptimizer: public Optimizator<T> {
private:
    size_t iterations = 500;
    size_t history_size = 10;
    T tolerance = T(1) / T(1000000);
    T line_search_decay = T(1) / T(2);
    T line_search_control = T(1) / T(10000);

    Matrix<T> compute_gradient(const LearningModel<T>* model, const Matrix<T>& weights,
        const Matrix<T>& X, const Matrix<T>& X_transposed, const Matrix<T>& y) const;

    T compute_loss(const LearningModel<T>* model, const Matrix<T>& weights,
        const Matrix<T>& X, const Matrix<T>& y) const;

    T dot(const Matrix<T>& column_a, const Matrix<T>& column_b) const;

public:
    void optimize(LearningModel<T>* learning_model) const override;
    std::unique_ptr<Optimizator<T>> clone() const override;
    OptimizatorType get_type() const override;
};

template <class T>
Matrix<T> LBFGSOptimizer<T>::compute_gradient(const LearningModel<T>* model, const Matrix<T>& weights,
    const Matrix<T>& X, const Matrix<T>& X_transposed, const Matrix<T>& y) const {
    switch (model->get_type()) {
        case LearningModelType::LinearRegression:
        case LearningModelType::PolynomialRegression: {
            Matrix<T> predictions = X * weights;
            Matrix<T> errors = predictions - y;
            return X_transposed * errors;
        }
        case LearningModelType::LogisticRegression: {
            Matrix<T> scores = X * weights;
            Matrix<T> predictions;
            predictions.resize_dims(scores.get_rows_count(), 1);
            for (size_t i = 0; i < scores.get_rows_count(); i++) {
                predictions[i][0] = T(1) / (T(1) + std::exp(-scores[i][0]));
            }
            Matrix<T> errors = predictions - y;
            return X_transposed * errors;
        }
        default: {
            throw std::runtime_error("L-BFGS does not support this model type");
        }
    }
}

template <class T>
T LBFGSOptimizer<T>::compute_loss(const LearningModel<T>* model, const Matrix<T>& weights,
    const Matrix<T>& X, const Matrix<T>& y) const {
    Matrix<T> scores = X * weights;
    size_t samples = scores.get_rows_count();
    T loss = T(0);

    switch (model->get_type()) {
        case LearningModelType::LinearRegression:
        case LearningModelType::PolynomialRegression: {
            for (size_t i = 0; i < samples; i++) {
                T error = scores[i][0] - y[i][0];
                loss += error * error;
            }
            return loss / T(2);
        }
        case LearningModelType::LogisticRegression: {
            for (size_t i = 0; i < samples; i++) {
                T probability = T(1) / (T(1) + std::exp(-scores[i][0]));
                T clamped = std::min(std::max(probability, T(1) / T(1000000000)), T(1) - T(1) / T(1000000000));
                loss -= y[i][0] * std::log(clamped) + (T(1) - y[i][0]) * std::log(T(1) - clamped);
            }
            return loss;
        }
        default: {
            throw std::runtime_error("L-BFGS does not support this model type");
        }
    }
}

template <class T>
T LBFGSOptimizer<T>::dot(const Matrix<T>& column_a, const Matrix<T>& column_b) const {
    T sum = T(0);
    for (size_t i = 0; i < column_a.get_rows_count(); i++) {
        sum += column_a[i][0] * column_b[i][0];
    }

    return sum;
}

template <class T>
void LBFGSOptimizer<T>::optimize(LearningModel<T>* learning_model) const {
    if (!learning_model) {
        throw std::runtime_error("Nullptr detected");
    }

    const SupervisedLearningModel<T>* supervised_model = dynamic_cast<const SupervisedLearningModel<T>*>(learning_model);
    if (!supervised_model) {
        throw std::runtime_error("L-BFGS requires a supervised learning model");
    }

    const Matrix<T>* X = learning_model->get_X();
    const Matrix<T>* y = supervised_model->get_y();
    Matrix<T> X_transposed = X->transpose();

    Matrix<T> weights = learning_model->get_weights();
    Matrix<T> gradient = this->compute_gradient(learning_model, weights, *X, X_transposed, *y);

    std::deque<Matrix<T>> weight_deltas;
    std::deque<Matrix<T>> gradient_deltas;
    std::deque<T> rhos;

    for (size_t iteration = 0; iteration < this->iterations; iteration++) {
        if (std::sqrt(this->dot(gradient, gradient)) < this->tolerance) {
            break;
        }

        Matrix<T> q = gradient;
        size_t stored = weight_deltas.size();
        std::vector<T> alphas(stored, T(0));

        for (size_t i = stored; i-- > 0;) {
            T alpha = rhos[i] * this->dot(weight_deltas[i], q);
            alphas[i] = alpha;
            q = q - gradient_deltas[i] * alpha;
        }

        T scale = T(1);
        if (stored > 0) {
            T last = this->dot(gradient_deltas.back(), gradient_deltas.back());
            if (last > this->tolerance) {
                scale = this->dot(weight_deltas.back(), gradient_deltas.back()) / last;
            }
        }
        Matrix<T> direction = q * scale;

        for (size_t i = 0; i < stored; i++) {
            T beta = rhos[i] * this->dot(gradient_deltas[i], direction);
            direction = direction + weight_deltas[i] * (alphas[i] - beta);
        }

        direction *= T(-1);

        T directional_derivative = this->dot(gradient, direction);
        if (directional_derivative >= T(0)) {
            direction = gradient * T(-1);
            directional_derivative = this->dot(gradient, direction);
            weight_deltas.clear();
            gradient_deltas.clear();
            rhos.clear();
        }

        T step = T(1);
        T current_loss = this->compute_loss(learning_model, weights, *X, *y);
        Matrix<T> next_weights = weights + direction * step;
        while (this->compute_loss(learning_model, next_weights, *X, *y)
            > current_loss + this->line_search_control * step * directional_derivative) {
            step *= this->line_search_decay;
            if (step < this->tolerance) {
                break;
            }
            next_weights = weights + direction * step;
        }

        Matrix<T> next_gradient = this->compute_gradient(learning_model, next_weights, *X, X_transposed, *y);

        Matrix<T> weight_delta = next_weights - weights;
        Matrix<T> gradient_delta = next_gradient - gradient;

        T curvature = this->dot(gradient_delta, weight_delta);
        if (curvature > this->tolerance) {
            if (weight_deltas.size() == this->history_size) {
                weight_deltas.pop_front();
                gradient_deltas.pop_front();
                rhos.pop_front();
            }
            weight_deltas.push_back(weight_delta);
            gradient_deltas.push_back(gradient_delta);
            rhos.push_back(T(1) / curvature);
        }

        weights = next_weights;
        gradient = next_gradient;
    }

    learning_model->set_weights(weights);
}

template <class T>
std::unique_ptr<Optimizator<T>> LBFGSOptimizer<T>::clone() const {
    return std::make_unique<LBFGSOptimizer<T>>(*this);
}

template <class T>
OptimizatorType LBFGSOptimizer<T>::get_type() const {
    return OptimizatorType::LBFGS;
}
