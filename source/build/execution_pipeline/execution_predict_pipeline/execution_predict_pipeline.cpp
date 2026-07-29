#include <memory>
#include <vector>

#include "source/utils.hpp"
#include "source/build/utils.h"
#include "source/data/data_structures/matrix/matrix.hpp"
#include "source/learning_models/metrics/metrics_reporter.hpp"
#include "source/learning_models/model_factory/model_factory.hpp"
#include "source/learning_models/scaling/scaler_factory/scaler_factory.hpp"
#include "source/data/input_output/writers/writer_factory/writer_factory.hpp"
#include "source/build/model_io/text_model_serializer/text_model_serializer.h"
#include "source/build/execution_pipeline/execution_predict_pipeline/execution_predict_pipeline.h"

ExecutionPredictPipeline::ExecutionPredictPipeline(const std::string& model_in, const std::string& input_csv,
    const std::string& output_csv):
    model_in(model_in), input_csv(input_csv), output_csv(output_csv) {}

void ExecutionPredictPipeline::execute() const {
    LoadedModel loaded = TextModelSerializer(this->model_in).load();

    std::unique_ptr<Scaler<float>> scaler;
    if (loaded.has_scaler) {
        scaler = ScalerFactory<float>().create(loaded.scaler_type);
        scaler->set_parameters(loaded.scaler_parameters);
    }

    Matrix<float> dummy_X, dummy_y;
    dummy_X.resize_dims(1, 1);
    dummy_y.resize_dims(1, 1);

    bool clustering = METRICS::is_clustering<float>(loaded.type);

    Matrix<float> X, unused_y;
    if (clustering) {
        BUILD::read_features_scaled(this->input_csv.c_str(), X, scaler.get());
    } else {
        BUILD::split_features_target_scaled(this->input_csv.c_str(), needs_bias(loaded.type), X, unused_y, scaler.get());
    }

    std::vector<double> hyperparameters;
    switch (loaded.type) {
    case LearningModelType::PolynomialRegression:
        hyperparameters = { (double)loaded.degree };
        break;
    case LearningModelType::RANSAC:
        hyperparameters = { 1, 1, 0 };
        break;
    case LearningModelType::DecisionTree:
        hyperparameters = { 1 };
        break;
    case LearningModelType::RandomForest:
        hyperparameters = { 1, 1 };
        break;
    case LearningModelType::KMeans:
        hyperparameters = { 1, 1 };
        break;
    case LearningModelType::DBSCAN:
        hyperparameters = { 1, 1 };
        break;
    case LearningModelType::HierarchicalClustering:
        hyperparameters = { 1 };
        break;
    default:
        break;
    }

    ModelFactory<float> factory;
    std::unique_ptr<LearningModel<float>> model(factory.create(loaded.type, &dummy_X, &dummy_y, hyperparameters));

    model->set_weights(loaded.weights);
    Matrix<float> predictions = model->predict(X);
    WriterFactory<CSVTable>().create(WriterType::CSV_WRITER, this->output_csv)->write(BUILD::matrix_to_table(predictions));
}

ExecutionPipelineType ExecutionPredictPipeline::get_type() const {
    return ExecutionPipelineType::ExecutionPredictPipeline;
}