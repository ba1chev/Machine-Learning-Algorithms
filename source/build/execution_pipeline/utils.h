#pragma once

#include "source/learning_models/learning_model_type.h"

namespace TRAIN {
    bool model_supports_optimizer(LearningModelType type);
}