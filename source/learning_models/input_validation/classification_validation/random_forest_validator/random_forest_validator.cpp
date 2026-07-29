#include <cmath>

#include "source/data/utils.h"
#include "source/learning_models/input_validation/classification_validation/random_forest_validator/random_forest_validator.h"

bool RandomForestValidator::validate(int argc, const char* argv[]) const {
    if (argc != 4 || !argv || !argv[1] || !argv[2] || !argv[3]) return false;

    double n_trees = 0;
    if (!NUM::is_number(argv[2], n_trees)) return false;
    if (n_trees < 1 || std::floor(n_trees) != n_trees) return false;

    double max_depth = 0;
    if (!NUM::is_number(argv[3], max_depth)) return false;
    if (max_depth < 1 || std::floor(max_depth) != max_depth) return false;

    return true;
}

InputValidatorType RandomForestValidator::get_type() const {
    return InputValidatorType::RandomForestValidator;
}
