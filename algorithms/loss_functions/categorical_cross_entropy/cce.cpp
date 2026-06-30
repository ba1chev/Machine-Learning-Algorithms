#include <cmath>
#include <vector>
#include <stdexcept>

typedef std::vector<float>(*MulticlassFunction)(const std::vector<float>&);

float categorical_cross_entropy(const std::vector<std::vector<float>>& input, const std::vector<size_t>& labels,
    MulticlassFunction function) {
    if (input.size() != labels.size()) {
        throw std::runtime_error("Invalid input data");
    }

    float result = 0;
    const float epsilon = 1e-7f;

    for (size_t i = 0; i < input.size(); i++) {
        std::vector<float> prediction = function(input[i]);

        if (labels[i] >= prediction.size()) {
            throw std::runtime_error("Invalid input data");
        }

        float probability = prediction[labels[i]];

        if (probability < epsilon) {
            probability = epsilon;
        }
        if (probability > 1 - epsilon) {
            probability = 1 - epsilon;
        }

        result += std::log(probability);
    }

    return -result / input.size();
}
