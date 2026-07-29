#pragma once

#include <vector>
#include <memory>
#include <iomanip>
#include <iostream>

#include "source/learning_models/metrics/metric.hpp"
#include "source/learning_models/metrics/metric_type.h"
#include "source/learning_models/learning_model_type.h"
#include "source/data/data_structures/matrix/matrix.hpp"
#include "source/learning_models/metrics/metric_factory/metric_factory.hpp"

namespace METRICS {
    template <class T>
    bool is_classification(LearningModelType type) {
        return type == LearningModelType::LogisticRegression
            || type == LearningModelType::DecisionTree
            || type == LearningModelType::RandomForest;
    }

    template <class T>
    bool is_clustering(LearningModelType type) {
        return type == LearningModelType::KMeans
            || type == LearningModelType::DBSCAN
            || type == LearningModelType::HierarchicalClustering;
    }

    template <class T>
    std::vector<std::unique_ptr<Metric<T>>> make_metrics(LearningModelType type) {
        std::vector<MetricType> metric_types = is_classification<T>(type)
            ? std::vector<MetricType>{ MetricType::ACCURACY, MetricType::PRECISION,
                MetricType::RECALL, MetricType::F1_SCORE }
            : std::vector<MetricType>{ MetricType::MEAN_SQUARED_ERROR, MetricType::ROOT_MEAN_SQUARED_ERROR,
                MetricType::MEAN_ABSOLUTE_ERROR, MetricType::R_SQUARED };

        MetricFactory<T> factory;
        std::vector<std::unique_ptr<Metric<T>>> metrics;
        for (MetricType metric_type : metric_types) {
            metrics.push_back(factory.create(metric_type));
        }

        return metrics;
    }

    template <class T>
    void report(LearningModelType type, const Matrix<T>& y_true, const Matrix<T>& y_pred) {
        std::vector<std::unique_ptr<Metric<T>>> metrics = make_metrics<T>(type);

        std::cout << "Training metrics:" << std::endl;
        for (const std::unique_ptr<Metric<T>>& metric : metrics) {
            std::cout << "  " << std::left << std::setw(12) << metric->get_name()
                << metric->compute(y_true, y_pred) << std::endl;
        }
    }
}
