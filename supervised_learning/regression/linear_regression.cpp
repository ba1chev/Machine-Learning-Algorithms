#include <cmath>
#include <vector>
#include <stdexcept>

typedef float(*Function)(const std::vector<float>&);

std::vector<float> linear_regression_params = { 0, 0, 0, 0 };
std::vector<std::vector<float>> X = { { 1, 2, 3 }, { 2, 3, 4 }, { 3, 4, 5 } };
std::vector<float> Y = { 14, 20, 26 };

float linear_regression(const std::vector<float>& input) {
    if (input.size() + 1 != linear_regression_params.size()) {
        throw std::runtime_error("Invalid input data");
    }

    float result = 0;

    for (size_t i = 0; i < input.size(); i++) {
        result += input[i] * linear_regression_params[i];
    }

    result += linear_regression_params[input.size()];
    return result;
}

float mean_squared_error(const std::vector<std::vector<float>>& input, const std::vector<float>& labels, 
    Function function) {
    float result = 0;

    for (size_t i = 0; i < input.size(); i++) {
        float diff_value = function(input[i]) - labels[i];
        result += diff_value * diff_value;
    }

    return result / input.size();
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
    size_t count_of_features = starting_position.size();
    std::vector<float> result = starting_position;

    for (size_t i = 0; i < count_of_iterations; i++) {
        std::vector<float> current_derivative = calculate_derivative_at(function, result);

        for (size_t j = 0; j < count_of_features; j++) {
            result[j] -= learning_rate * current_derivative[j];
        }
    }

    return result;
}

float loss_function(const std::vector<float>& params) {
    linear_regression_params = params;
    return mean_squared_error(X, Y, linear_regression);
}

int main() {
    std::vector<float> start = {0, 0, 0, 0};
    auto result = gradient_descent(loss_function, start, 5000, 0.01);
    linear_regression_params = result;

    return 0;
}