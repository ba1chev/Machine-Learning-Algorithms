#pragma once

#include "source/build/execution_pipeline/pipeline_factory/abstract_pipeline_factory.h"

class PipelineFactory: public AbstractPipelineFactory {
public:
    std::unique_ptr<ExecutionPipeline> create(ExecutionPipelineType pipeline_type,
        LearningModelType type, int argc, const char* argv[],
        const std::string& optimizer_type, const std::string& scaler_type, const std::string& model_out,
        const std::string& model_in, const std::string& input_csv, const std::string& output_csv) const override;
};
