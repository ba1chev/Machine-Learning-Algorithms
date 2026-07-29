#include "source/build/execution_pipeline/utils.h"

bool TRAIN::model_supports_optimizer(LearningModelType type) {
    switch (type) {
        case LearningModelType::LinearRegression:
        case LearningModelType::PolynomialRegression:
        case LearningModelType::LogisticRegression:
        case LearningModelType::RANSAC: {
            return true;
        }
        default: return false;
    }
}