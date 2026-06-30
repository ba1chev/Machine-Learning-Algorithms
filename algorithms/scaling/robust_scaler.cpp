#include <algorithm>
#include <vector>
#include <stdexcept>

std::vector<std::vector<float>> X = { { 1, 10 }, { 2, 20 }, { 3, 30 }, { 4, 40 }, { 5, 50 }, { 100, 1000 } };
std::vector<float> robust_scaler_medians;
std::vector<float> robust_scaler_iqrs;

float compute_quantile(std::vector<float> values, float quantile) {
    std::sort(values.begin(), values.end());
    float position = quantile * (values.size() - 1);
    size_t lower_index = static_cast<size_t>(position);
    size_t upper_index = lower_index + 1;

    if (upper_index >= values.size()) {
        return values[lower_index];
    }

    float fraction = position - lower_index;
    return values[lower_index] + fraction * (values[upper_index] - values[lower_index]);
}

std::vector<std::vector<float>> robust_scaler_fit(const std::vector<std::vector<float>>& input) {
    if (input.empty()) {
        throw std::runtime_error("Invalid input data");
    }

    size_t count_of_samples = input.size();
    size_t count_of_features = input[0].size();

    robust_scaler_medians.assign(count_of_features, 0);
    robust_scaler_iqrs.assign(count_of_features, 0);

    for (size_t j = 0; j < count_of_features; j++) {
        std::vector<float> feature_values(count_of_samples, 0);

        for (size_t i = 0; i < count_of_samples; i++) {
            if (input[i].size() != count_of_features) {
                throw std::runtime_error("Invalid input data");
            }

            feature_values[i] = input[i][j];
        }

        robust_scaler_medians[j] = compute_quantile(feature_values, 0.5f);
        robust_scaler_iqrs[j] = compute_quantile(feature_values, 0.75f) - compute_quantile(feature_values, 0.25f);
    }

    std::vector<std::vector<float>> result(count_of_samples, std::vector<float>(count_of_features, 0));

    for (size_t i = 0; i < count_of_samples; i++) {
        for (size_t j = 0; j < count_of_features; j++) {
            if (robust_scaler_iqrs[j] == 0) {
                result[i][j] = 0;
            }
            else {
                result[i][j] = (input[i][j] - robust_scaler_medians[j]) / robust_scaler_iqrs[j];
            }
        }
    }

    return result;
}

std::vector<float> robust_scaler(const std::vector<float>& input) {
    if (input.size() != robust_scaler_medians.size()) {
        throw std::runtime_error("Invalid input data");
    }

    std::vector<float> result(input.size(), 0);

    for (size_t i = 0; i < input.size(); i++) {
        if (robust_scaler_iqrs[i] == 0) {
            result[i] = 0;
        }
        else {
            result[i] = (input[i] - robust_scaler_medians[i]) / robust_scaler_iqrs[i];
        }
    }

    return result;
}

int main() {
    std::vector<std::vector<float>> scaled = robust_scaler_fit(X);
    std::vector<float> prediction = robust_scaler({ 3, 25 });

    return 0;
}
