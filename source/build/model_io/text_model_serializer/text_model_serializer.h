#pragma once

#include <string>

#include "source/build/model_io/model_serializer.h"

class TextModelSerializer : public ModelSerializer {
public:
    TextModelSerializer(const std::string& filename);
    void save(LearningModelType type, size_t degree, const Matrix<float>& weights,
        bool has_scaler, ScalerType scaler_type, const Matrix<float>& scaler_parameters) const override;
    LoadedModel load() const override;
    ModelSerializerType get_type() const override;
};
