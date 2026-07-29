#pragma once

#include <memory>
#include <stdexcept>

#include "source/learning_models/scaling/normal_scaler/normal_scaler.hpp"
#include "source/learning_models/scaling/min_max_scaler/min_max_scaler.hpp"
#include "source/learning_models/scaling/scaler_factory/abstract_scaler_factory.hpp"

template <class T>
class ScalerFactory: public AbstractScalerFactory<T> {
public:
    std::unique_ptr<Scaler<T>> create(ScalerType scaler_type) const override;
};

template <class T>
std::unique_ptr<Scaler<T>> ScalerFactory<T>::create(ScalerType scaler_type) const {
    switch (scaler_type) {
        case ScalerType::NORMAL_SCALER: return std::make_unique<NormalScaler<T>>();
        case ScalerType::MIN_MAX_SCALER: return std::make_unique<MinMaxScaler<T>>();
        default: throw std::runtime_error("Unsupported scaler type");
    }
}
