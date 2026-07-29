#pragma once

#include "source/learning_models/learning_model_type.h"
#include "source/learning_models/scaling/scaler_type.h"
#include "source/data/data_structures/matrix/matrix.hpp"

struct LoadedModel {
    size_t degree = 0;
    LearningModelType type = LearningModelType::LinearRegression;
    Matrix<float> weights;

    bool has_scaler = false;
    ScalerType scaler_type = ScalerType::NORMAL_SCALER;
    Matrix<float> scaler_parameters;
};