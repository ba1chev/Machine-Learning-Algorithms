#include <cmath>

#include "source/data/utils.h"
#include "source/learning_models/input_validation/regression_validation/ransac_validator/ransac_validator.h"

bool RansacValidator::validate(int argc, const char* argv[]) const {
    if (argc != 5 || !argv || !argv[1] || !argv[2] || !argv[3] || !argv[4]) {
        return false;
    }

    double iterations = 0;
    if (!NUM::is_number(argv[2], iterations) || iterations < 1 || std::floor(iterations) != iterations) {
        return false;
    }

    double sample_size = 0;
    if (!NUM::is_number(argv[3], sample_size) || sample_size < 1 || std::floor(sample_size) != sample_size) {
        return false;
    }

    double inlier_threshold = 0;
    if (!NUM::is_number(argv[4], inlier_threshold) || inlier_threshold <= 0) {
        return false;
    }

    return true;
}

InputValidatorType RansacValidator::get_type() const {
    return InputValidatorType::RANSACValidator;
}