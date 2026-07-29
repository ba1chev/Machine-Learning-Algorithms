#include <memory>
#include <stdexcept>

#include "source/build/execution_pipeline/pipeline_factory/pipeline_factory.h"
#include "source/build/execution_pipeline/execution_train_pipeline/execution_train_pipeline.h"
#include "source/build/execution_pipeline/execution_predict_pipeline/execution_predict_pipeline.h"

std::unique_ptr<ExecutionPipeline> PipelineFactory::create(ExecutionPipelineType pipeline_type,
    LearningModelType type, int argc, const char* argv[],
    const std::string& optimizer_type, const std::string& scaler_type, const std::string& model_out,
    const std::string& model_in, const std::string& input_csv, const std::string& output_csv) const {
    switch (pipeline_type) {
        case ExecutionPipelineType::ExecutionTrainPipeline: {
            return std::make_unique<ExecutionTrainPipeline>(type, argc, argv, optimizer_type, scaler_type, model_out);
        }
        case ExecutionPipelineType::ExecutionPredictPipeline: {
            return std::make_unique<ExecutionPredictPipeline>(model_in, input_csv, output_csv);
        }
        default: throw std::runtime_error("Unsupported execution pipeline type");
    }
}
