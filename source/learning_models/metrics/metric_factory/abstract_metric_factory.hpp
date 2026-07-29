#pragma once

#include <memory>

#include "source/learning_models/metrics/metric.hpp"
#include "source/learning_models/metrics/metric_type.h"

template <class T>
class AbstractMetricFactory {
public:
    virtual std::unique_ptr<Metric<T>> create(MetricType metric_type) const = 0;
    virtual ~AbstractMetricFactory() = default;
};