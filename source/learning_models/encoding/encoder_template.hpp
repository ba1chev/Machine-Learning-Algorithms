#pragma once

#include <memory>

#include "source/learning_models/encoding/encoder.h"
#include "source/data/data_structures/matrix/matrix.hpp"
#include "source/learning_models/encoding/encoder_type.h"

template <class U, class V>
class EncoderTemplate: public Encoder {
public:
    virtual V encode(const U& data) const = 0;
    virtual std::unique_ptr<EncoderTemplate<U, V>> clone() const = 0;
};