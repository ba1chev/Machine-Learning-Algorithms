#pragma once

#include <memory>

#include "source/learning_models/encoding/encoder_type.h"
#include "source/learning_models/encoding/encoder_template.hpp"

template <class U, class V>
class AbstractEncoderFactory {
public:
    virtual std::unique_ptr<EncoderTemplate<U, V>> create(EncoderType encoder_type) const = 0;
    virtual ~AbstractEncoderFactory() = default;
};
