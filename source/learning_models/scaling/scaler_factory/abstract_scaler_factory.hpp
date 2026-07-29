#pragma once

#include <memory>

#include "source/learning_models/scaling/scaler.hpp"
#include "source/learning_models/scaling/scaler_type.h"

template <class T>
class AbstractScalerFactory {
public:
    virtual std::unique_ptr<Scaler<T>> create(ScalerType scaler_type) const = 0;
    virtual ~AbstractScalerFactory() = default;
};
