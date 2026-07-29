#pragma once

#include <vector>
#include <cstdlib>

#include "source/learning_models/learning_model_type.h"
#include "source/build/execution_pipeline/execution_pipeline_type.h"

class ExecutionPipeline {
protected:
    static bool needs_bias(LearningModelType type);
    static std::vector<double> parse_hyperparameters(int argc, const char* argv[]);

public:
    virtual void execute() const = 0;
    virtual ExecutionPipelineType get_type() const = 0;
    virtual ~ExecutionPipeline() = default;
};
