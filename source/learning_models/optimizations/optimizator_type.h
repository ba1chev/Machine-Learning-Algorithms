#pragma once

enum class OptimizatorType {
    GradientDescent = (1 << 0),
    BFGS = (1 << 1),
    LBFGS = (1 << 2)
};