#pragma once

#include <memory>
#include <stdexcept>

#include "source/learning_models/optimizations/bfgs/bfgs.hpp"
#include "source/learning_models/optimizations/bfgs/l_bfgs.hpp"
#include "source/learning_models/optimizations/gradient_descent/gradient_descent.hpp"
#include "source/learning_models/optimizations/optimizator_factory/abstract_optimizator_factory.hpp"

template <class T>
class OptimizatorFactory: public AbstractOptimizatorFactory<T> {
public:
    std::unique_ptr<Optimizator<T>> create(OptimizatorType optimizator_type) const override;
};

template <class T>
std::unique_ptr<Optimizator<T>> OptimizatorFactory<T>::create(OptimizatorType optimizator_type) const {
    switch (optimizator_type) {
        case OptimizatorType::GradientDescent: return std::make_unique<GradientDescent<T>>();
        case OptimizatorType::BFGS: return std::make_unique<BFGSOptimizer<T>>();
        case OptimizatorType::LBFGS: return std::make_unique<LBFGSOptimizer<T>>();
        default: throw std::runtime_error("Unsupported optimizator type");
    }
}
