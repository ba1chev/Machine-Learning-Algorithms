#pragma once

#include <string>

#include "source/learning_models/learning_model_type.h"

class ExecutionBuilder {
public:
    void predict(const std::string& model_in, const std::string& input_csv, const std::string& output_csv) const;
    void train(LearningModelType type, int argc, const char* argv[],
        const std::string& optimizer_type, const std::string& scaler_type, const std::string& model_out) const;
};
