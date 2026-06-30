#include <cmath>
#include <vector>
#include <stdexcept>

typedef float(*Function)(const std::vector<float>&);

float huber_delta = 1.0f;
float regularization_lambda = 0.01f;

float huber_loss(const std::vector<std::vector<float>>& input, const std::vector<float>& labels,
    const std::vector<float>& weights, Function function) {
    if (input.size() != labels.size()) {
        throw std::runtime_error("Invalid input data");
    }

    float result = 0;

    for (size_t i = 0; i < input.size(); i++) {
        float diff_value = function(input[i]) - labels[i];
        float absolute_diff = std::abs(diff_value);

        if (absolute_diff <= huber_delta) {
            result += 0.5f * diff_value * diff_value;
        }
        else {
            result += huber_delta * (absolute_diff - 0.5f * huber_delta);
        }
    }

    result /= input.size();

    float l2_penalty = 0;

    for (size_t i = 0; i < weights.size(); i++) {
        l2_penalty += weights[i] * weights[i];
    }

    result += regularization_lambda * l2_penalty;
    return result;
}
