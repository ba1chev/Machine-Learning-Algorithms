#pragma once

#include "source/learning_models/input_validation/input_validator.h"

class HierarchicalClusteringValidator: public InputValidator {
public:
    bool validate(int argc, const char* argv[]) const override;
    InputValidatorType get_type() const override;
};
