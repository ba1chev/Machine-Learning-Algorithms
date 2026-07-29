#include <cmath>

#include "source/data/utils.h"
#include "source/learning_models/input_validation/clustering_validation/kmeans_validator/kmeans_validator.h"

bool KMeansValidator::validate(int argc, const char* argv[]) const {
    if (argc != 3 || !argv || !argv[1] || !argv[2]) {
        return false;
    }

    double k = 0;
    if (!NUM::is_number(argv[2], k)) {
        return false;
    }

    if (k < 1 || std::floor(k) != k) {
        return false;
    }

    return true;
}

InputValidatorType KMeansValidator::get_type() const {
    return InputValidatorType::KMeansValidator;
}
