#include <vector>
#include <stdexcept>

std::vector<std::vector<float>> X = { { 1, 10 }, { 2, 20 }, { 3, 30 }, { 4, 40 }, { 5, 50 } };
std::vector<float> min_max_scaler_minimums;
std::vector<float> min_max_scaler_maximums;

std::vector<std::vector<float>> min_max_scaler_fit(const std::vector<std::vector<float>>& input) {
    if (input.empty()) {
        throw std::runtime_error("Invalid input data");
    }

    size_t count_of_samples = input.size();
    size_t count_of_features = input[0].size();

    min_max_scaler_minimums = input[0];
    min_max_scaler_maximums = input[0];

    for (size_t i = 1; i < count_of_samples; i++) {
        if (input[i].size() != count_of_features) {
            throw std::runtime_error("Invalid input data");
        }

        for (size_t j = 0; j < count_of_features; j++) {
            if (input[i][j] < min_max_scaler_minimums[j]) {
                min_max_scaler_minimums[j] = input[i][j];
            }
            if (input[i][j] > min_max_scaler_maximums[j]) {
                min_max_scaler_maximums[j] = input[i][j];
            }
        }
    }

    std::vector<std::vector<float>> result(count_of_samples, std::vector<float>(count_of_features, 0));

    for (size_t i = 0; i < count_of_samples; i++) {
        for (size_t j = 0; j < count_of_features; j++) {
            float range = min_max_scaler_maximums[j] - min_max_scaler_minimums[j];

            if (range == 0) {
                result[i][j] = 0;
            }
            else {
                result[i][j] = (input[i][j] - min_max_scaler_minimums[j]) / range;
            }
        }
    }

    return result;
}

std::vector<float> min_max_scaler(const std::vector<float>& input) {
    if (input.size() != min_max_scaler_minimums.size()) {
        throw std::runtime_error("Invalid input data");
    }

    std::vector<float> result(input.size(), 0);

    for (size_t i = 0; i < input.size(); i++) {
        float range = min_max_scaler_maximums[i] - min_max_scaler_minimums[i];

        if (range == 0) {
            result[i] = 0;
        }
        else {
            result[i] = (input[i] - min_max_scaler_minimums[i]) / range;
        }
    }

    return result;
}

int main() {
    std::vector<std::vector<float>> scaled = min_max_scaler_fit(X);
    std::vector<float> prediction = min_max_scaler({ 3, 25 });

    return 0;
}
