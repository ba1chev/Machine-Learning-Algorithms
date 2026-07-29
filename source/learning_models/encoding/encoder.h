#pragma once

#include "source/learning_models/encoding/encoder_type.h"

class Encoder {
public:
    virtual EncoderType get_type() const = 0;
    virtual ~Encoder() = default;
};