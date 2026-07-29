#pragma once

#include "source/learning_models/learning_model.hpp"
#include "source/data/data_structures/matrix/matrix.hpp"

template <class T>
class SupervisedLearningModel: public LearningModel<T> {
protected:
    const Matrix<T>* y = nullptr;

public:
    SupervisedLearningModel(const Matrix<T>* X, const Matrix<T>* y);
    const Matrix<T>* get_y() const;
};

template <class T>
SupervisedLearningModel<T>::SupervisedLearningModel(const Matrix<T>* X, const Matrix<T>* y): LearningModel<T>(X) {
    if (!y) {
        throw std::runtime_error("Nullptr detected");
    }

    this->y = y;
}

template <class T>
const Matrix<T>* SupervisedLearningModel<T>::get_y() const {
    return this->y;
}
