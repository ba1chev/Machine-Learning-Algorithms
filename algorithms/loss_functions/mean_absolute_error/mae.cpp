#include <cmath>
#include <vector>
#include <stdexcept>

typedef float(*Function)(const std::vector<float>&);

float mean_absolute_error(const std::vector<std::vector<float>>& input, const std::vector<float>& labels,
    Function function) {
    if (input.size() != labels.size()) {
        throw std::runtime_error("Invalid input data");
    }

    float result = 0;

    for (size_t i = 0; i < input.size(); i++) {
        result += std::abs(function(input[i]) - labels[i]);
    }

    return result / input.size();
}
