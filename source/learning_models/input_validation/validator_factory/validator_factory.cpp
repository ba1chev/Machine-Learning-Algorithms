#include <memory>
#include <stdexcept>

#include "source/learning_models/input_validation/validator_factory/validator_factory.h"
#include "source/learning_models/input_validation/regression_validation/ransac_validator/ransac_validator.h"
#include "source/learning_models/input_validation/clustering_validation/kmeans_validator/kmeans_validator.h"
#include "source/learning_models/input_validation/clustering_validation/dbscan_validator/dbscan_validator.h"
#include "source/learning_models/input_validation/classification_validation/decision_tree_validator/decision_tree_validator.h"
#include "source/learning_models/input_validation/classification_validation/random_forest_validator/random_forest_validator.h"
#include "source/learning_models/input_validation/regression_validation/linear_regression_validator/linear_regression_validator.h"
#include "source/learning_models/input_validation/regression_validation/polynomial_regression_validator/polynomial_regression_validator.h"
#include "source/learning_models/input_validation/classification_validation/logistic_regression_validator/logistic_regression_validator.h"
#include "source/learning_models/input_validation/clustering_validation/hierarchical_clustering_validator/hierarchical_clustering_validator.h"

std::unique_ptr<InputValidator> ValidatorFactory::create(InputValidatorType validator_type) const {
    switch (validator_type) {
        case InputValidatorType::LinearRegressionValidator: return std::make_unique<LinearRegressionValidator>();
        case InputValidatorType::PolynomialRegressionValidator: return std::make_unique<PolynomialRegressionValidator>();
        case InputValidatorType::RANSACValidator: return std::make_unique<RansacValidator>();
        case InputValidatorType::LogisticRegressionValidator: return std::make_unique<LogisticRegressionValidator>();
        case InputValidatorType::DecisionTreeValidator: return std::make_unique<DecisionTreeValidator>();
        case InputValidatorType::RandomForestValidator: return std::make_unique<RandomForestValidator>();
        case InputValidatorType::KMeansValidator: return std::make_unique<KMeansValidator>();
        case InputValidatorType::DBSCANValidator: return std::make_unique<DBSCANValidator>();
        case InputValidatorType::HierarchicalClusteringValidator: return std::make_unique<HierarchicalClusteringValidator>();
        default: throw std::runtime_error("Unsupported input validator type");
    }
}

InputValidatorType ValidatorFactory::convert_type(LearningModelType model_type) {
    InputValidatorType validator_type;

    switch (model_type) {
        case LearningModelType::LinearRegression: validator_type = InputValidatorType::LinearRegressionValidator; break;
        case LearningModelType::PolynomialRegression: validator_type = InputValidatorType::PolynomialRegressionValidator; break;
        case LearningModelType::RANSAC: validator_type = InputValidatorType::RANSACValidator; break;
        case LearningModelType::LogisticRegression: validator_type = InputValidatorType::LogisticRegressionValidator; break;
        case LearningModelType::DecisionTree: validator_type = InputValidatorType::DecisionTreeValidator; break;
        case LearningModelType::RandomForest: validator_type = InputValidatorType::RandomForestValidator; break;
        case LearningModelType::KMeans: validator_type = InputValidatorType::KMeansValidator; break;
        case LearningModelType::DBSCAN: validator_type = InputValidatorType::DBSCANValidator; break;
        case LearningModelType::HierarchicalClustering: validator_type = InputValidatorType::HierarchicalClusteringValidator; break;
        default: throw std::runtime_error("Not supported learning model type");
    }

    return validator_type;
}
