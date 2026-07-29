#include <cmath>

#include "source/data/utils.h"
#include "source/learning_models/input_validation/clustering_validation/hierarchical_clustering_validator/hierarchical_clustering_validator.h"

bool HierarchicalClusteringValidator::validate(int argc, const char* argv[]) const {
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

InputValidatorType HierarchicalClusteringValidator::get_type() const {
    return InputValidatorType::HierarchicalClusteringValidator;
}
