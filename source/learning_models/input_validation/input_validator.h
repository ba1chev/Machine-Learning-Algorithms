#pragma once

#include "source/learning_models/input_validation/input_validator_type.h"

class InputValidator {
public:
    virtual bool validate(int argc, const char* argv[]) const = 0;
    virtual InputValidatorType get_type() const = 0;
    virtual ~InputValidator() = default;
};