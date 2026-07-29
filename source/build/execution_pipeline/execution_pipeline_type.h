#pragma once

enum class ExecutionPipelineType {
    ExecutionTrainPipeline = (1 << 0),
    ExecutionPredictPipeline = (1 << 1)
};