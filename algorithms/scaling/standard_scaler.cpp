#include <cmath>
#include <vector>
#include <stdexcept>

std::vector<std::vector<float>> X = { { 1, 10 }, { 2, 20 }, { 3, 30 }, { 4, 40 }, { 5, 50 } };
std::vector<float> standard_scaler_means;
std::vector<float> standard_scaler_deviations;

std::vector<std::vector<float>> standard_scaler_fit(const std::vector<std::vector<float>>& input) {
    if (input.empty()) {
        throw std::runtime_error("Invalid input data");
    }

    size_t count_of_samples = input.size();
    size_t count_of_features = input[0].size();

    standard_scaler_means.assign(count_of_features, 0);
    standard_scaler_deviations.assign(count_of_features, 0);

    for (size_t i = 0; i < count_of_samples; i++) {
        if (input[i].size() != count_of_features) {
            throw std::runtime_error("Invalid input data");
        }

        for (size_t j = 0; j < count_of_features; j++) {
            standard_scaler_means[j] += input[i][j];
        }
    }

    for (size_t j = 0; j < count_of_features; j++) {
        standard_scaler_means[j] /= count_of_samples;
    }

    for (size_t i = 0; i < count_of_samples; i++) {
        for (size_t j = 0; j < count_of_features; j++) {
            float diff_value = input[i][j] - standard_scaler_means[j];
            standard_scaler_deviations[j] += diff_value * diff_value;
        }
    }

    for (size_t j = 0; j < count_of_features; j++) {
        standard_scaler_deviations[j] = std::sqrt(standard_scaler_deviations[j] / count_of_samples);
    }

    std::vector<std::vector<float>> result(count_of_samples, std::vector<float>(count_of_features, 0));

    for (size_t i = 0; i < count_of_samples; i++) {
        for (size_t j = 0; j < count_of_features; j++) {
            if (standard_scaler_deviations[j] == 0) {
                result[i][j] = 0;
            }
            else {
                result[i][j] = (input[i][j] - standard_scaler_means[j]) / standard_scaler_deviations[j];
            }
        }
    }

    return result;
}

std::vector<float> standard_scaler(const std::vector<float>& input) {
    if (input.size() != standard_scaler_means.size()) {
        throw std::runtime_error("Invalid input data");
    }

    std::vector<float> result(input.size(), 0);

    for (size_t i = 0; i < input.size(); i++) {
        if (standard_scaler_deviations[i] == 0) {
            result[i] = 0;
        }
        else {
            result[i] = (input[i] - standard_scaler_means[i]) / standard_scaler_deviations[i];
        }
    }

    return result;
}

int main() {
    std::vector<std::vector<float>> scaled = standard_scaler_fit(X);
    std::vector<float> prediction = standard_scaler({ 3, 25 });

    return 0;
}
