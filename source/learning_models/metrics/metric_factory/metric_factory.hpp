#pragma once

#include <memory>
#include <stdexcept>

#include "source/learning_models/metrics/classification/recall/recall.hpp"
#include "source/learning_models/metrics/regression/r_squared/r_squared.hpp"
#include "source/learning_models/metrics/classification/f1_score/f1_score.hpp"
#include "source/learning_models/metrics/classification/accuracy/accuracy.hpp"
#include "source/learning_models/metrics/classification/precision/precision.hpp"
#include "source/learning_models/metrics/metric_factory/abstract_metric_factory.hpp"
#include "source/learning_models/metrics/regression/mean_squared_error/mean_squared_error.hpp"
#include "source/learning_models/metrics/regression/mean_absolute_error/mean_absolute_error.hpp"
#include "source/learning_models/metrics/regression/root_mean_squared_error/root_mean_squared_error.hpp"

template <class T>
class MetricFactory: public AbstractMetricFactory<T> {
public:
    std::unique_ptr<Metric<T>> create(MetricType metric_type) const override;
};

template <class T>
std::unique_ptr<Metric<T>> MetricFactory<T>::create(MetricType metric_type) const {
    switch (metric_type) {
        case MetricType::MEAN_SQUARED_ERROR: return std::make_unique<MeanSquaredError<T>>();
        case MetricType::ROOT_MEAN_SQUARED_ERROR: return std::make_unique<RootMeanSquaredError<T>>();
        case MetricType::MEAN_ABSOLUTE_ERROR: return std::make_unique<MeanAbsoluteError<T>>();
        case MetricType::R_SQUARED: return std::make_unique<RSquared<T>>();
        case MetricType::ACCURACY: return std::make_unique<Accuracy<T>>();
        case MetricType::PRECISION: return std::make_unique<Precision<T>>();
        case MetricType::RECALL: return std::make_unique<Recall<T>>();
        case MetricType::F1_SCORE: return std::make_unique<F1Score<T>>();
        default: throw std::runtime_error("Unsupported metric type");
    }
}
