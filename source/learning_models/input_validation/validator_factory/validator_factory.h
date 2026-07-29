#pragma once

#include <memory>

#include "source/learning_models/learning_model_type.h"
#include "source/learning_models/input_validation/validator_factory/abstract_validator_factory.h"

class ValidatorFactory: public AbstractValidatorFactory {
public:
    std::unique_ptr<InputValidator> create(InputValidatorType validator_type) const override;
    static InputValidatorType convert_type(LearningModelType model_type);
};