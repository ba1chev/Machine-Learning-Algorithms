#pragma once

#include <vector>
#include <memory>

#include "source/learning_models/learning_model.hpp"
#include "source/learning_models/learning_model_type.h"
#include "source/data/data_structures/matrix/matrix.hpp"

template <class T>
class AbstractModelFactory {
public:
    virtual std::unique_ptr<LearningModel<T>> create(LearningModelType model_type, const Matrix<T>* X, const Matrix<T>* y,
        const std::vector<double>& hyperparameters) const = 0;
    virtual ~AbstractModelFactory() = default;
};
