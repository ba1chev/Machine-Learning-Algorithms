#pragma once

#include <memory>

#include "source/learning_models/learning_model_type.h"
#include "source/learning_models/supervised_learning/supervised_learning_model.hpp"
#include "source/learning_models/optimizations/gradient_descent/gradient_descent.hpp"

template <class T>
class LinearRegression: public SupervisedLearningModel<T> {
public:
    LinearRegression(const Matrix<T>* X, const Matrix<T>* y): SupervisedLearningModel<T>(X, y) {}

    void fit() override;
    Matrix<T> predict(const Matrix<T>& X) const override;
    LearningModelType get_type() const override;
    std::unique_ptr<LearningModel<T>> clone() const override;
};

template <class T>
void LinearRegression<T>::fit() {
    if (this->get_optimizer()) {
        this->get_optimizer()->optimize(this);
        return;
    }

    GradientDescent<T> optimizator;
    optimizator.optimize(this);
}

template <class T>
Matrix<T> LinearRegression<T>::predict(const Matrix<T>& X) const {
    if (X.get_cols_count() != this->weights.get_rows_count()) {
        throw std::runtime_error("Feature count does not match trained weights");
    }

    return X * this->weights;
}

template <class T>
LearningModelType LinearRegression<T>::get_type() const {
    return LearningModelType::LinearRegression;
}

template <class T>
std::unique_ptr<LearningModel<T>> LinearRegression<T>::clone() const {
    return std::make_unique<LinearRegression<T>>(*this);
}