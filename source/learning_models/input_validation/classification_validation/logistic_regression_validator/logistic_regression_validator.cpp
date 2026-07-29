#include "source/learning_models/input_validation/classification_validation/logistic_regression_validator/logistic_regression_validator.h"

bool LogisticRegressionValidator::validate(int argc, const char* argv[]) const {
    if (argc != 2 || !argv || !argv[1]) {
        return false;
    }

    return true;
}

InputValidatorType LogisticRegressionValidator::get_type() const {
    return InputValidatorType::LogisticRegressionValidator;
}