#pragma once

#include <memory>

#include "source/learning_models/learning_model_type.h"
#include "source/learning_models/supervised_learning/supervised_learning_model.hpp"
#include "source/learning_models/optimizations/gradient_descent/gradient_descent.hpp"

template <class T>
class PolynomialRegression: public SupervisedLearningModel<T> {
private:
    size_t degree;
    Matrix<T> expanded_X;

    static Matrix<T> expand(const Matrix<T>& raw_X, size_t degree);

public:
    PolynomialRegression(const Matrix<T>* X, const Matrix<T>* y, size_t degree);

    void fit() override;
    Matrix<T> predict(const Matrix<T>& X) const override;
    LearningModelType get_type() const override;
    std::unique_ptr<LearningModel<T>> clone() const override;
};

template <class T>
Matrix<T> PolynomialRegression<T>::expand(const Matrix<T>& raw_X, size_t degree) {
    size_t rows = raw_X.get_rows_count();
    size_t raw_cols = raw_X.get_cols_count();

    Matrix<T> result;
    result.resize_dims(rows, raw_cols * degree + 1);

    for (size_t i = 0; i < rows; i++) {
        size_t col = 0;
        for (size_t f = 0; f < raw_cols; f++) {
            T power = T(1);
            for (size_t d = 0; d < degree; d++) {
                power *= raw_X[i][f];
                result[i][col] = power;
                col += 1;
            }
        }
        result[i][col] = T(1);
    }

    return result;
}

template <class T>
PolynomialRegression<T>::PolynomialRegression(const Matrix<T>* X, const Matrix<T>* y, size_t degree)
    : SupervisedLearningModel<T>(X, y), degree(degree) {
    if (degree == 0) {
        throw std::runtime_error("Polynomial degree must be at least 1");
    }

    this->expanded_X = expand(*X, degree);
    this->set_X(&this->expanded_X);
}

template <class T>
void PolynomialRegression<T>::fit() {
    if (this->get_optimizer()) {
        this->get_optimizer()->optimize(this);
        return;
    }

    GradientDescent<T> optimizator;
    optimizator.optimize(this);
}

template <class T>
Matrix<T> PolynomialRegression<T>::predict(const Matrix<T>& X) const {
    Matrix<T> expanded = expand(X, this->degree);

    if (expanded.get_cols_count() != this->weights.get_rows_count()) {
        throw std::runtime_error("Feature count does not match trained weights");
    }

    return expanded * this->weights;
}

template <class T>
LearningModelType PolynomialRegression<T>::get_type() const {
    return LearningModelType::PolynomialRegression;
}

template <class T>
std::unique_ptr<LearningModel<T>> PolynomialRegression<T>::clone() const {
    return std::make_unique<PolynomialRegression<T>>(*this);
}
