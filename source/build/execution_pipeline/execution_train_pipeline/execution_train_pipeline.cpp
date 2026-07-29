#include <memory>
#include <vector>
#include <iostream>
#include <stdexcept>

#include "source/utils.hpp"
#include "source/build/utils.h"
#include "source/build/execution_pipeline/utils.h"
#include "source/data/data_structures/matrix/matrix.hpp"
#include "source/learning_models/metrics/metrics_reporter.hpp"
#include "source/learning_models/model_factory/model_factory.hpp"
#include "source/learning_models/scaling/scaler_factory/scaler_factory.hpp"
#include "source/build/model_io/text_model_serializer/text_model_serializer.h"
#include "source/learning_models/input_validation/validator_factory/validator_factory.h"
#include "source/learning_models/optimizations/optimizator_factory/optimizator_factory.hpp"
#include "source/build/execution_pipeline/execution_train_pipeline/execution_train_pipeline.h"

ExecutionTrainPipeline::ExecutionTrainPipeline(LearningModelType type, int argc, const char* argv[],
    const std::string& optimizer_type, const std::string& scaler_type, const std::string& model_out):
    argc(argc), argv(argv), type(type), optimizer_type(optimizer_type), scaler_type(scaler_type), model_out(model_out) {}

void ExecutionTrainPipeline::execute() const {
    bool has_scaler = !INPUT::is_scaler_none(this->scaler_type.c_str());
    std::unique_ptr<Scaler<float>> scaler;
    if (has_scaler) {
        scaler = ScalerFactory<float>().create(INPUT::parse_scaler_type(this->scaler_type.c_str()));
    }

    std::unique_ptr<InputValidator> validator(
        ValidatorFactory().create(ValidatorFactory::convert_type(this->type)));
    if (!validator->validate(this->argc, this->argv)) {
        throw std::runtime_error("Invalid input data");
    }

    std::vector<double> hyperparameters = parse_hyperparameters(this->argc, this->argv);

    OptimizatorType optimizer = INPUT::parse_optimizator_type(this->optimizer_type.c_str());
    bool wants_non_default = optimizer != OptimizatorType::GradientDescent;
    if (wants_non_default && !TRAIN::model_supports_optimizer(this->type)) {
        throw std::runtime_error("Model does not support optimizer selection");
    }

    bool clustering = METRICS::is_clustering<float>(this->type);

    Matrix<float> X, y;
    if (clustering) {
        BUILD::read_features(this->argv[1], X, scaler.get());
    } else {
        BUILD::split_features_target(this->argv[1], needs_bias(this->type), X, y, scaler.get());
    }

    ModelFactory<float> factory;
    std::unique_ptr<LearningModel<float>> model(factory.create(this->type, &X, &y, hyperparameters));
    if (TRAIN::model_supports_optimizer(this->type)) {
        model->set_optimizer(OptimizatorFactory<float>().create(optimizer));
    }
    model->fit();
    Matrix<float> weights = model->get_weights();

    if (clustering) {
        std::cout << "Clusters: " << weights.get_rows_count() << std::endl;
    } else {
        METRICS::report<float>(this->type, y, model->predict(X));
    }

    size_t degree = 0;
    if (this->type == LearningModelType::PolynomialRegression) {
        degree = (size_t)hyperparameters[0];
    }

    ScalerType resolved_scaler_type = ScalerType::NORMAL_SCALER;
    Matrix<float> scaler_parameters;
    if (has_scaler) {
        resolved_scaler_type = scaler->get_type();
        scaler_parameters = scaler->get_parameters();
    }

    TextModelSerializer(this->model_out).save(this->type, degree, weights, has_scaler, resolved_scaler_type, scaler_parameters);
}

ExecutionPipelineType ExecutionTrainPipeline::get_type() const {
    return ExecutionPipelineType::ExecutionTrainPipeline;
}