#include <cmath>
#include <vector>
#include <stdexcept>

typedef float(*Function)(const std::vector<float>&);

std::vector<float> polynomial_regression_params = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
std::vector<std::vector<float>> X = { { 1, 2, 3 }, { 2, 3, 4 }, { 3, 4, 5 } };
std::vector<float> Y = { 14, 20, 26 };

std::vector<float> expand(const std::vector<float>& x) {
    std::vector<float> features;

    for (float current_value : x) {
        features.push_back(current_value);
    }

    for (size_t i = 0; i < x.size(); i++) {
        for (size_t j = i; j < x.size(); j++) {
            features.push_back(x[i] * x[j]);
        }
    }

    return features;
}

float model(const std::vector<float>& input) {
    std::vector<float> x = expand(input);

    if (x.size() + 1 != polynomial_regression_params.size()) {
        throw std::runtime_error("Invalid input data");
    }

    float result = 0;

    for (size_t i = 0; i < x.size(); i++) {
        result += x[i] * polynomial_regression_params[i];
    }

    result += polynomial_regression_params[x.size()];
    return result;
}

float mean_squared_error(const std::vector<std::vector<float>>& input, const std::vector<float>& labels, Function function) {
    float result = 0;

    for (size_t i = 0; i < input.size(); i++) {
        float diff_value = function(input[i]) - labels[i];
        result += diff_value * diff_value;
    }

    return result / input.size();
}

std::vector<float> calculate_derivative_at(Function function, const std::vector<float>& sample, const float& precision = 0.001) {
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

std::vector<float> gradient_descent(Function function, 
    const std::vector<float>& starting_position, size_t iterations = 1000, float learning_rate = 0.01) {
    std::vector<float> params = starting_position;

    for (size_t i = 0; i < iterations; i++) {
        std::vector<float> grad = calculate_derivative_at(function, params);

        for (size_t j = 0; j < params.size(); j++) {
            params[j] -= learning_rate * grad[j];
        }
    }

    return params;
}

float loss_function(const std::vector<float>& params) {
    polynomial_regression_params = params;
    return mean_squared_error(X, Y, model);
}

int main() {
    std::vector<float> start = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    auto result = gradient_descent(loss_function, start, 5000, 0.01);
    polynomial_regression_params = result;
    return 0;
}