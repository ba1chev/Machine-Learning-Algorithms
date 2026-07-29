#pragma once

#include <memory>

#include "source/learning_models/optimizations/optimizator.hpp"
#include "source/learning_models/optimizations/optimizator_type.h"

template <class T>
class AbstractOptimizatorFactory {
public:
    virtual std::unique_ptr<Optimizator<T>> create(OptimizatorType optimizator_type) const = 0;
    virtual ~AbstractOptimizatorFactory() = default;
};
