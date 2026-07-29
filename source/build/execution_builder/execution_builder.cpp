#include "source/build/execution_builder/execution_builder.h"
#include "source/build/execution_pipeline/execution_pipeline_type.h"
#include "source/build/execution_pipeline/pipeline_factory/pipeline_factory.h"

void ExecutionBuilder::train(LearningModelType type, int argc, const char* argv[],
    const std::string& optimizer_type, const std::string& scaler_type, const std::string& model_out) const {
    PipelineFactory().create(ExecutionPipelineType::ExecutionTrainPipeline,
        type, argc, argv, optimizer_type, scaler_type, model_out, "", "", "")->execute();
}

void ExecutionBuilder::predict(const std::string& model_in, const std::string& input_csv, const std::string& output_csv) const {
    PipelineFactory().create(ExecutionPipelineType::ExecutionPredictPipeline,
        LearningModelType::LinearRegression, 0, nullptr, "", "", "", model_in, input_csv, output_csv)->execute();
}
