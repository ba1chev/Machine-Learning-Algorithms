#include "source/build/execution_pipeline/execution_pipeline.h"

bool ExecutionPipeline::needs_bias(LearningModelType type) {
    switch (type) {
        case LearningModelType::LinearRegression:
        case LearningModelType::RANSAC:
        case LearningModelType::LogisticRegression:
            return true;
        default: {
            return false;
        }
    }
}

std::vector<double> ExecutionPipeline::parse_hyperparameters(int argc, const char* argv[]) {
    std::vector<double> hyperparameters;
    for (int i = 2; i < argc; i++) {
        hyperparameters.push_back(std::strtod(argv[i], nullptr));
    }
    return hyperparameters;
}
