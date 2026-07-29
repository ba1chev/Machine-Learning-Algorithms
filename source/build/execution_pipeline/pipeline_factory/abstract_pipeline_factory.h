#pragma once

#include <string>
#include <memory>

#include "source/learning_models/learning_model_type.h"
#include "source/build/execution_pipeline/execution_pipeline.h"
#include "source/build/execution_pipeline/execution_pipeline_type.h"

class AbstractPipelineFactory {
public:
    virtual std::unique_ptr<ExecutionPipeline> create(ExecutionPipelineType pipeline_type,
        LearningModelType type, int argc, const char* argv[],
        const std::string& optimizer_type, const std::string& scaler_type, const std::string& model_out,
        const std::string& model_in, const std::string& input_csv, const std::string& output_csv) const = 0;
    virtual ~AbstractPipelineFactory() = default;
};
