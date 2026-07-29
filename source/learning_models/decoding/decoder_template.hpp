#pragma once

#include <memory>

#include "source/learning_models/decoding/decoder.h"
#include "source/data/data_structures/matrix/matrix.hpp"

template <class U, class V>
class DecoderTemplate: public Decoder {
public:
    virtual V decode(const U& data) const = 0;
    virtual std::unique_ptr<DecoderTemplate<U, V>> clone() const = 0;
};