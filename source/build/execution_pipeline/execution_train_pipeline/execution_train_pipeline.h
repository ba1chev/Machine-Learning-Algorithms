#pragma once

#include <string>

#include "source/learning_models/learning_model_type.h"
#include "source/build/execution_pipeline/execution_pipeline.h"

class ExecutionTrainPipeline: public ExecutionPipeline {
private:
    int argc = 0;
    const char** argv = nullptr;

    LearningModelType type;
    std::string optimizer_type;
    std::string scaler_type;
    std::string model_out;

public:
    ExecutionTrainPipeline(LearningModelType type, int argc, const char* argv[],
        const std::string& optimizer_type, const std::string& scaler_type, const std::string& model_out);

    void execute() const override;
    ExecutionPipelineType get_type() const override;
};
