#pragma once

#include <cmath>
#include <memory>

#include "source/learning_models/learning_model_type.h"
#include "source/learning_models/supervised_learning/supervised_learning_model.hpp"
#include "source/learning_models/optimizations/gradient_descent/gradient_descent.hpp"

template <class T>
class LogisticRegression: public SupervisedLearningModel<T> {
public:
    LogisticRegression(const Matrix<T>* X, const Matrix<T>* y): SupervisedLearningModel<T>(X, y) {}

    void fit() override;
    Matrix<T> predict(const Matrix<T>& X) const override;
    LearningModelType get_type() const override;
    std::unique_ptr<LearningModel<T>> clone() const override;
};

template <class T>
void LogisticRegression<T>::fit() {
    if (this->get_optimizer()) {
        this->get_optimizer()->optimize(this);
        return;
    }

    GradientDescent<T> optimizator;
    optimizator.optimize(this);
}

template <class T>
Matrix<T> LogisticRegression<T>::predict(const Matrix<T>& X) const {
    if (X.get_cols_count() != this->weights.get_rows_count()) {
        throw std::runtime_error("Feature count does not match trained weights");
    }

    Matrix<T> scores = X * this->weights;

    Matrix<T> labels;
    labels.resize_dims(scores.get_rows_count(), 1);

    for (size_t i = 0; i < scores.get_rows_count(); i++) {
        T probability = T(1) / (T(1) + std::exp(-scores[i][0]));
        labels[i][0] = probability >= T(1) / T(2) ? T(1) : T(0);
    }

    return labels;
}

template <class T>
LearningModelType LogisticRegression<T>::get_type() const {
    return LearningModelType::LogisticRegression;
}

template <class T>
std::unique_ptr<LearningModel<T>> LogisticRegression<T>::clone() const {
    return std::make_unique<LogisticRegression<T>>(*this);
}
