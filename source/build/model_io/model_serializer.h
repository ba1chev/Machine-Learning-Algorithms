#pragma once

#include <string>

#include "source/build/model_io/loaded_model.h"
#include "source/learning_models/scaling/scaler_type.h"
#include "source/build/model_io/model_serializer_type.h"
#include "source/data/data_structures/matrix/matrix.hpp"

class ModelSerializer {
protected:
    std::string filename;

public:
    ModelSerializer(const std::string& filename);
    virtual void save(LearningModelType type, size_t degree, const Matrix<float>& weights,
        bool has_scaler, ScalerType scaler_type, const Matrix<float>& scaler_parameters) const = 0;
    virtual LoadedModel load() const = 0;
    virtual ModelSerializerType get_type() const = 0;
    virtual ~ModelSerializer() = default;
};
