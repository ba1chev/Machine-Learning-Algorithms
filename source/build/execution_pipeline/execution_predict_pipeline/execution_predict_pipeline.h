#pragma once

#include <string>

#include "source/build/execution_pipeline/execution_pipeline.h"

class ExecutionPredictPipeline: public ExecutionPipeline {
private:
    std::string model_in;
    std::string input_csv;
    std::string output_csv;

public:
    ExecutionPredictPipeline(const std::string& model_in, const std::string& input_csv,
        const std::string& output_csv);

    void execute() const override;
    ExecutionPipelineType get_type() const override;
};
