#pragma once

#include <memory>
#include <vector>
#include <iostream>

#include "source/learning_models/learning_model_type.h"
#include "source/data/data_structures/matrix/matrix.hpp"

template <class T>
class Optimizator;

template <class T>
class LearningModel {
protected:
    Matrix<T> weights;
    const Matrix<T>* X = nullptr;
    std::unique_ptr<Optimizator<T>> optimizer;

    void set_X(const Matrix<T>* X);

public:
    LearningModel(const Matrix<T>* X);
    LearningModel(const LearningModel<T>& other);
    LearningModel<T>& operator = (const LearningModel<T>& other);

    const Matrix<T>* get_X() const;
    virtual const Matrix<T>& get_weights() const;
    virtual void set_weights(const Matrix<T>& weights);

    void set_optimizer(std::unique_ptr<Optimizator<T>> optimizer);
    Optimizator<T>* get_optimizer() const;

    virtual void fit() = 0;
    virtual Matrix<T> predict(const Matrix<T>& X) const = 0;
    virtual LearningModelType get_type() const = 0;
    virtual std::unique_ptr<LearningModel<T>> clone() const = 0;
    virtual ~LearningModel() = default;
};

template <class T>
LearningModel<T>::LearningModel(const Matrix<T>* X) {
    if (!X) {
        throw std::runtime_error("Nullptr detected");
    }

    this->X = X;
    this->weights.resize_dims(X->get_cols_count(), 1);
}

template <class T>
LearningModel<T>::LearningModel(const LearningModel<T>& other) {
    this->weights = other.weights;
    this->X = other.X;
    this->optimizer = other.optimizer ? other.optimizer->clone() : nullptr;
}

template <class T>
LearningModel<T>& LearningModel<T>::operator = (const LearningModel<T>& other) {
    if (this != &other) {
        this->weights = other.weights;
        this->X = other.X;
        this->optimizer = other.optimizer ? other.optimizer->clone() : nullptr;
    }

    return *this;
}

template <class T>
const Matrix<T>* LearningModel<T>::get_X() const {
    return this->X;
}

template <class T>
const Matrix<T>& LearningModel<T>::get_weights() const {
    return this->weights;
}

template <class T>
void LearningModel<T>::set_weights(const Matrix<T>& weights) {
    this->weights = weights;
}

template <class T>
void LearningModel<T>::set_X(const Matrix<T>* X) {
    if (!X) {
        throw std::runtime_error("Nullptr detected");
    }

    this->X = X;
    this->weights.resize_dims(X->get_cols_count(), 1);
}

template <class T>
void LearningModel<T>::set_optimizer(std::unique_ptr<Optimizator<T>> optimizer) {
    this->optimizer = std::move(optimizer);
}

template <class T>
Optimizator<T>* LearningModel<T>::get_optimizer() const {
    return this->optimizer.get();
}
