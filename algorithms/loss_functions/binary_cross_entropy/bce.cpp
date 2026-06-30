#include <cmath>
#include <vector>
#include <stdexcept>

typedef float(*Function)(const std::vector<float>&);

float binary_cross_entropy(const std::vector<std::vector<float>>& input, const std::vector<float>& labels,
    Function function) {
    if (input.size() != labels.size()) {
        throw std::runtime_error("Invalid input data");
    }

    float result = 0;
    const float epsilon = 1e-7f;

    for (size_t i = 0; i < input.size(); i++) {
        float prediction = function(input[i]);

        if (prediction < epsilon) {
            prediction = epsilon;
        }
        if (prediction > 1 - epsilon) {
            prediction = 1 - epsilon;
        }

        result += labels[i] * std::log(prediction) + (1 - labels[i]) * std::log(1 - prediction);
    }

    return -result / input.size();
}
