#pragma once

#include <cmath>
#include <memory>

#include "source/learning_models/learning_model_type.h"
#include "source/learning_models/optimizations/optimizator.hpp"
#include "source/learning_models/optimizations/optimizator_type.h"
#include "source/learning_models/supervised_learning/supervised_learning_model.hpp"

template <class T>
class GradientDescent: public Optimizator<T> {
private:
    T learning_rate = T(1) / T(1000);
    size_t iterations = 1000;

public:
    void optimize(LearningModel<T>* learning_model) const override;
    std::unique_ptr<Optimizator<T>> clone() const override;
    OptimizatorType get_type() const override;
};

template <class T>
void GradientDescent<T>::optimize(LearningModel<T>* learning_model) const {
    if (!learning_model) {
        throw std::runtime_error("Nullptr detected");
    }

    const SupervisedLearningModel<T>* supervised_model = dynamic_cast<const SupervisedLearningModel<T>*>(learning_model);
    if (!supervised_model) {
        throw std::runtime_error("GradientDescent requires a supervised learning model");
    }

    const Matrix<T>* X = learning_model->get_X();
    const Matrix<T>* y = supervised_model->get_y();
    Matrix<T> X_transposed = X->transpose();

    size_t count_of_samples = X->get_rows_count();
    size_t count_of_features = X->get_cols_count();

    for (size_t iteration = 0; iteration < this->iterations; iteration++) {
        Matrix<T> weights = learning_model->get_weights();
        Matrix<T> gradient;

        switch (learning_model->get_type()) {
            case LearningModelType::LinearRegression:
            case LearningModelType::PolynomialRegression: {
                Matrix<T> predictions = (*X) * weights;
                Matrix<T> errors = predictions - (*y);
                gradient = X_transposed * errors;
                break;
            }
            case LearningModelType::LogisticRegression: {
                Matrix<T> scores = (*X) * weights;
                Matrix<T> predictions;
                predictions.resize_dims(scores.get_rows_count(), 1);
                for (size_t i = 0; i < scores.get_rows_count(); i++) {
                    predictions[i][0] = T(1) / (T(1) + std::exp(-scores[i][0]));
                }
                Matrix<T> errors = predictions - (*y);
                gradient = X_transposed * errors;
                break;
            }
            default: {
                throw std::runtime_error("GradientDescent does not support this model type");
            }
        }

        for (size_t i = 0; i < count_of_features; i++) {
            weights[i][0] -= this->learning_rate * gradient[i][0] / count_of_samples;
        }

        learning_model->set_weights(weights);
    }
}

template <class T>
std::unique_ptr<Optimizator<T>> GradientDescent<T>::clone() const {
    return std::make_unique<GradientDescent<T>>(*this);
}

template <class T>
OptimizatorType GradientDescent<T>::get_type() const {
    return OptimizatorType::GradientDescent;
}