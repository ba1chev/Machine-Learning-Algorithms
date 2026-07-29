#pragma once

#include <memory>

#include "source/learning_models/input_validation/input_validator.h"
#include "source/learning_models/input_validation/input_validator_type.h"

class AbstractValidatorFactory {
public:
    virtual std::unique_ptr<InputValidator> create(InputValidatorType validator_type) const = 0;
    virtual ~AbstractValidatorFactory() = default;
};