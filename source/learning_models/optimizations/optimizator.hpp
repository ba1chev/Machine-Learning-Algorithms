#pragma once

#include <memory>

#include "source/learning_models/learning_model.hpp"
#include "source/learning_models/optimizations/optimizator_type.h"

template <class T>
class Optimizator {
public:
    virtual void optimize(LearningModel<T>* learning_model) const = 0;
    virtual std::unique_ptr<Optimizator<T>> clone() const = 0;
    virtual OptimizatorType get_type() const = 0;
    virtual ~Optimizator() = default;
};