#pragma once

#include "source/learning_models/decoding/decoder_type.h"

class Decoder {
public:
    virtual DecoderType get_type() const = 0;
    virtual ~Decoder() = default;
};