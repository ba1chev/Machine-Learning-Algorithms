#include <cmath>
#include <vector>
#include <stdexcept>

std::vector<std::vector<float>> X = { { 1, -10 }, { -2, 20 }, { 3, -30 }, { 4, 40 }, { -5, 50 } };
std::vector<float> max_abs_scaler_maximums;

std::vector<std::vector<float>> max_abs_scaler_fit(const std::vector<std::vector<float>>& input) {
    if (input.empty()) {
        throw std::runtime_error("Invalid input data");
    }

    size_t count_of_samples = input.size();
    size_t count_of_features = input[0].size();

    max_abs_scaler_maximums.assign(count_of_features, 0);

    for (size_t i = 0; i < count_of_samples; i++) {
        if (input[i].size() != count_of_features) {
            throw std::runtime_error("Invalid input data");
        }

        for (size_t j = 0; j < count_of_features; j++) {
            float absolute_value = std::abs(input[i][j]);

            if (absolute_value > max_abs_scaler_maximums[j]) {
                max_abs_scaler_maximums[j] = absolute_value;
            }
        }
    }

    std::vector<std::vector<float>> result(count_of_samples, std::vector<float>(count_of_features, 0));

    for (size_t i = 0; i < count_of_samples; i++) {
        for (size_t j = 0; j < count_of_features; j++) {
            if (max_abs_scaler_maximums[j] == 0) {
                result[i][j] = 0;
            }
            else {
                result[i][j] = input[i][j] / max_abs_scaler_maximums[j];
            }
        }
    }

    return result;
}

std::vector<float> max_abs_scaler(const std::vector<float>& input) {
    if (input.size() != max_abs_scaler_maximums.size()) {
        throw std::runtime_error("Invalid input data");
    }

    std::vector<float> result(input.size(), 0);

    for (size_t i = 0; i < input.size(); i++) {
        if (max_abs_scaler_maximums[i] == 0) {
            result[i] = 0;
        }
        else {
            result[i] = input[i] / max_abs_scaler_maximums[i];
        }
    }

    return result;
}

int main() {
    std::vector<std::vector<float>> scaled = max_abs_scaler_fit(X);
    std::vector<float> prediction = max_abs_scaler({ 3, -25 });

    return 0;
}
