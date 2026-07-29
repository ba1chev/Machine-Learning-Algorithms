#pragma once

#include <memory>

#include "source/learning_models/decoding/decoder_type.h"
#include "source/learning_models/decoding/decoder_template.hpp"

template <class U, class V>
class AbstractDecoderFactory {
public:
    virtual std::unique_ptr<DecoderTemplate<U, V>> create(DecoderType decoder_type) const = 0;
    virtual ~AbstractDecoderFactory() = default;
};
