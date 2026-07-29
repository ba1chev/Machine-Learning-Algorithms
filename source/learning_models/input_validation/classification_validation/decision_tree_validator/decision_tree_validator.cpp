#include <cmath>

#include "source/data/utils.h"
#include "source/learning_models/input_validation/classification_validation/decision_tree_validator/decision_tree_validator.h"

bool DecisionTreeValidator::validate(int argc, const char* argv[]) const {
    if (argc != 3 || !argv || !argv[1] || !argv[2]) return false;

    double max_depth = 0;
    if (!NUM::is_number(argv[2], max_depth)) return false;
    if (max_depth < 1 || std::floor(max_depth) != max_depth) return false;

    return true;
}

InputValidatorType DecisionTreeValidator::get_type() const {
    return InputValidatorType::DecisionTreeValidator;
}
