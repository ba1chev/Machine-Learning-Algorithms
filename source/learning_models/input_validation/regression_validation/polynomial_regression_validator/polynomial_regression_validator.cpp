#include <cmath>

#include "source/data/utils.h"
#include "source/learning_models/input_validation/regression_validation/polynomial_regression_validator/polynomial_regression_validator.h"

bool PolynomialRegressionValidator::validate(int argc, const char* argv[]) const {
    if (argc != 3 || !argv || !argv[1] || !argv[2]) {
        return false;
    }

    double degree = 0;
    if (!NUM::is_number(argv[2], degree)) {
        return false;
    }

    if (degree < 1 || std::floor(degree) != degree) {
        return false;
    }

    return true;
}

InputValidatorType PolynomialRegressionValidator::get_type() const {
    return InputValidatorType::PolynomialRegressionValidator;
}
