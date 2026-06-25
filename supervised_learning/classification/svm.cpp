#include <vector>
#include <stdexcept>

typedef float (*Function)(const std::vector<float>&);

std::vector<float> svm_params = { 0, 0, 0, 0 };
std::vector<std::vector<float>> X = { { 1, 2, 3 }, { 2, 3, 4 }, { 3, 4, 5 }, { 5, 6, 7 } };
std::vector<float> Y = { -1, -1, 1, 1 };
float regularization = 0.01f;

float svm(const std::vector<float>& input) {
    if (input.size() + 1 != svm_params.size()) {
        throw std::runtime_error("Invalid input data");
    }

    float result = 0;

    for (size_t i = 0; i < input.size(); i++) {
        result += input[i] * svm_params[i];
    }

    result += svm_params[input.size()];
    return result;
}

float hinge_loss(const std::vector<std::vector<float>>& input, const std::vector<float>& labels,
    Function function) {
    float result = 0;

    for (size_t i = 0; i < input.size(); i++) {
        float margin = 1 - labels[i] * function(input[i]);

        if (margin > 0) {
            result += margin;
        }
    }

    float weight_penalty = 0;

    for (size_t i = 0; i + 1 < svm_params.size(); i++) {
        weight_penalty += svm_params[i] * svm_params[i];
    }

    return result / input.size() + regularization * weight_penalty;
}

std::vector<float> calculate_derivative_at(Function function, const std::vector<float>& sample,
    const float& precision = 0.001) {
    size_t count_of_features = sample.size();
    std::vector<float> result(count_of_features, 0);

    for (size_t i = 0; i < count_of_features; i++) {
        std::vector<float> left_part = sample;
        std::vector<float> right_part = sample;

        left_part[i] += precision;
        right_part[i] -= precision;

        result[i] = (function(left_part) - function(right_part)) / (2 * precision);
    }

    return result;
}

std::vector<float> gradient_descent(Function function, const std::vector<float>& starting_position,
    size_t count_of_iterations = 1000, const float& learning_rate = 0.01) {
    std::vector<float> result = starting_position;
    size_t count_of_features = result.size();

    for (size_t i = 0; i < count_of_iterations; i++) {
        std::vector<float> current_derivative = calculate_derivative_at(function, result);

        for (size_t j = 0; j < count_of_features; j++) {
            result[j] -= current_derivative[j] * learning_rate;
        }
    }

    return result;
}

float loss_function(const std::vector<float>& params) {
    svm_params = params;
    return hinge_loss(X, Y, svm);
}

int main() {
    std::vector<float> start = { 0, 0, 0, 0 };
    auto result = gradient_descent(loss_function, start, 5000, 0.01);
    svm_params = result;

    return 0;
}
