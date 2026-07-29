#include "source/learning_models/input_validation/regression_validation/linear_regression_validator/linear_regression_validator.h"

bool LinearRegressionValidator::validate(int argc, const char* argv[]) const {
    if (argc != 2 || !argv || !argv[1]) {
        return false;
    }

    return true;
}

InputValidatorType LinearRegressionValidator::get_type() const {
    return InputValidatorType::LinearRegressionValidator;
}