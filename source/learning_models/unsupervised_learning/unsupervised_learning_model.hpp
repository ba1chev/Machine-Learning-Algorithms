#pragma once

#include "source/learning_models/learning_model.hpp"
#include "source/data/data_structures/matrix/matrix.hpp"

template <class T>
class UnSupervisedLearningModel: public LearningModel<T> {
public:
    UnSupervisedLearningModel(const Matrix<T>* X);
};

template <class T>
UnSupervisedLearningModel<T>::UnSupervisedLearningModel(const Matrix<T>* X): LearningModel<T>(X) {}