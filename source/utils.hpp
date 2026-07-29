#pragma once

#include <string>
#include <cstring>
#include <stdexcept>

#include "source/learning_models/learning_model_type.h"
#include "source/learning_models/scaling/scaler_type.h"
#include "source/learning_models/optimizations/optimizator_type.h"

namespace INPUT {
    inline LearningModelType parse_type(const char* str);
    inline std::string type_to_string(LearningModelType type);

    inline bool is_scaler_none(const char* str);
    inline bool is_scaler_token(const char* str);
    inline ScalerType parse_scaler_type(const char* str);
    inline std::string scaler_type_to_string(ScalerType type);

    inline bool is_optimizer_token(const char* str);
    inline OptimizatorType parse_optimizator_type(const char* str);
    inline std::string optimizator_type_to_string(OptimizatorType type);
}

inline LearningModelType INPUT::parse_type(const char* str) {
    // regression models
    if (!strcmp(str, "linear_regression")) return LearningModelType::LinearRegression;
    else if (!strcmp(str, "polynomial_regression")) return LearningModelType::PolynomialRegression;
    else if (!strcmp(str, "ransac")) return LearningModelType::RANSAC;

    // classification models
    else if (!strcmp(str, "logistic_regression")) return LearningModelType::LogisticRegression;
    else if (!strcmp(str, "decision_tree")) return LearningModelType::DecisionTree;
    else if (!strcmp(str, "random_forest")) return LearningModelType::RandomForest;

    // clustering
    else if (!strcmp(str, "k_means")) return LearningModelType::KMeans;
    else if (!strcmp(str, "dbscan")) return LearningModelType::DBSCAN;
    else if (!strcmp(str, "hierarchical_clustering")) return LearningModelType::HierarchicalClustering;

    // error
    else throw std::runtime_error("Unsupported learning model type");
}

inline std::string INPUT::type_to_string(LearningModelType type) {
    switch (type) {
        // regression models
        case LearningModelType::LinearRegression: return "linear_regression";
        case LearningModelType::PolynomialRegression: return "polynomial_regression";
        case LearningModelType::RANSAC: return "ransac";

        // classification models
        case LearningModelType::LogisticRegression: return "logistic_regression";
        case LearningModelType::DecisionTree: return "decision_tree";
        case LearningModelType::RandomForest: return "random_forest";

        // clustering
        case LearningModelType::KMeans: return "k_means";
        case LearningModelType::DBSCAN: return "dbscan";
        case LearningModelType::HierarchicalClustering: return "hierarchical_clustering";

        // error
        default: throw std::runtime_error("Unsupported learning model type");
    }
}

inline bool INPUT::is_scaler_none(const char* str) {
    return !strcmp(str, "none");
}

inline bool INPUT::is_scaler_token(const char* str) {
    return !strcmp(str, "none") || !strcmp(str, "normal") || !strcmp(str, "min_max");
}

inline ScalerType INPUT::parse_scaler_type(const char* str) {
    if (!strcmp(str, "normal")) return ScalerType::NORMAL_SCALER;
    else if (!strcmp(str, "min_max")) return ScalerType::MIN_MAX_SCALER;
    else throw std::runtime_error("Unsupported scaler type");
}

inline std::string INPUT::scaler_type_to_string(ScalerType type) {
    switch (type) {
        case ScalerType::NORMAL_SCALER: return "normal";
        case ScalerType::MIN_MAX_SCALER: return "min_max";
        default: throw std::runtime_error("Unsupported scaler type");
    }
}

inline bool INPUT::is_optimizer_token(const char* str) {
    return !strcmp(str, "gradient_descent") || !strcmp(str, "bfgs") || !strcmp(str, "lbfgs");
}

inline OptimizatorType INPUT::parse_optimizator_type(const char* str) {
    if (!strcmp(str, "gradient_descent")) return OptimizatorType::GradientDescent;
    else if (!strcmp(str, "bfgs")) return OptimizatorType::BFGS;
    else if (!strcmp(str, "lbfgs")) return OptimizatorType::LBFGS;
    else throw std::runtime_error("Unsupported optimizator type");
}

inline std::string INPUT::optimizator_type_to_string(OptimizatorType type) {
    switch (type) {
        case OptimizatorType::GradientDescent: return "gradient_descent";
        case OptimizatorType::BFGS: return "bfgs";
        case OptimizatorType::LBFGS: return "lbfgs";
        default: throw std::runtime_error("Unsupported optimizator type");
    }
}
