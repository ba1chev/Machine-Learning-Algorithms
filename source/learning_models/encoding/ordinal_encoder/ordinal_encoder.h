#pragma once

#include <memory>
#include <string>

#include "source/learning_models/encoding/encoder_template.hpp"

class OrdinalEncoder: public EncoderTemplate<Matrix<std::string>, Matrix<float>> {
public:
    Matrix<float> encode(const Matrix<std::string>& data) const override;
    std::unique_ptr<EncoderTemplate<Matrix<std::string>, Matrix<float>>> clone() const override;
    EncoderType get_type() const override;
};
