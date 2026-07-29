#include <cmath>

#include "source/data/utils.h"
#include "source/learning_models/input_validation/clustering_validation/dbscan_validator/dbscan_validator.h"

bool DBSCANValidator::validate(int argc, const char* argv[]) const {
    if (argc != 4 || !argv || !argv[1] || !argv[2] || !argv[3]) {
        return false;
    }

    double eps = 0;
    if (!NUM::is_number(argv[2], eps) || eps <= 0) {
        return false;
    }

    double min_samples = 0;
    if (!NUM::is_number(argv[3], min_samples)) {
        return false;
    }

    if (min_samples < 1 || std::floor(min_samples) != min_samples) {
        return false;
    }

    return true;
}

InputValidatorType DBSCANValidator::get_type() const {
    return InputValidatorType::DBSCANValidator;
}
