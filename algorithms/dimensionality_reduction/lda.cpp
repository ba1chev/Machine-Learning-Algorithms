#include <cmath>
#include <vector>
#include <stdexcept>

std::vector<std::vector<float>> X = { { 2, 3 }, { 1, 1 }, { 3, 4 }, { 5, 6 }, { 6, 5 }, { 7, 8 } };
std::vector<float> Y = { 0, 0, 0, 1, 1, 1 };
std::vector<float> lda_means_negative;
std::vector<float> lda_means_positive;
std::vector<float> lda_projection;

std::vector<std::vector<float>> invert_matrix(std::vector<std::vector<float>> matrix) {
    size_t size = matrix.size();
    std::vector<std::vector<float>> result(size, std::vector<float>(size, 0));

    for (size_t i = 0; i < size; i++) {
        result[i][i] = 1;
    }

    for (size_t i = 0; i < size; i++) {
        float pivot_value = matrix[i][i];

        if (std::abs(pivot_value) < 1e-7f) {
            throw std::runtime_error("Invalid input data");
        }

        for (size_t j = 0; j < size; j++) {
            matrix[i][j] /= pivot_value;
            result[i][j] /= pivot_value;
        }

        for (size_t k = 0; k < size; k++) {
            if (k == i) {
                continue;
            }

            float factor = matrix[k][i];

            for (size_t j = 0; j < size; j++) {
                matrix[k][j] -= factor * matrix[i][j];
                result[k][j] -= factor * result[i][j];
            }
        }
    }

    return result;
}

void compute_class_mean(const std::vector<std::vector<float>>& input, const std::vector<float>& labels,
    float target_class, std::vector<float>& mean_value) {
    size_t count_of_features = input[0].size();
    mean_value.assign(count_of_features, 0);
    size_t count_of_samples = 0;

    for (size_t i = 0; i < input.size(); i++) {
        if (labels[i] != target_class) {
            continue;
        }

        for (size_t j = 0; j < count_of_features; j++) {
            mean_value[j] += input[i][j];
        }

        count_of_samples++;
    }

    if (count_of_samples == 0) {
        throw std::runtime_error("Invalid input data");
    }

    for (size_t j = 0; j < count_of_features; j++) {
        mean_value[j] /= count_of_samples;
    }
}

void lda_fit(const std::vector<std::vector<float>>& input, const std::vector<float>& labels) {
    if (input.empty() || input.size() != labels.size()) {
        throw std::runtime_error("Invalid input data");
    }

    size_t count_of_features = input[0].size();
    compute_class_mean(input, labels, 0, lda_means_negative);
    compute_class_mean(input, labels, 1, lda_means_positive);

    std::vector<std::vector<float>> within_class_scatter(count_of_features, std::vector<float>(count_of_features, 0));

    for (size_t i = 0; i < input.size(); i++) {
        const std::vector<float>& class_mean = labels[i] == 1 ? lda_means_positive : lda_means_negative;

        for (size_t j = 0; j < count_of_features; j++) {
            for (size_t k = 0; k < count_of_features; k++) {
                within_class_scatter[j][k] += (input[i][j] - class_mean[j]) * (input[i][k] - class_mean[k]);
            }
        }
    }

    std::vector<std::vector<float>> inverted_scatter = invert_matrix(within_class_scatter);
    std::vector<float> mean_difference(count_of_features, 0);

    for (size_t j = 0; j < count_of_features; j++) {
        mean_difference[j] = lda_means_positive[j] - lda_means_negative[j];
    }

    lda_projection.assign(count_of_features, 0);

    for (size_t j = 0; j < count_of_features; j++) {
        for (size_t k = 0; k < count_of_features; k++) {
            lda_projection[j] += inverted_scatter[j][k] * mean_difference[k];
        }
    }

    float norm_value = 0;

    for (size_t j = 0; j < count_of_features; j++) {
        norm_value += lda_projection[j] * lda_projection[j];
    }

    norm_value = std::sqrt(norm_value);

    if (norm_value < 1e-7f) {
        throw std::runtime_error("Invalid input data");
    }

    for (size_t j = 0; j < count_of_features; j++) {
        lda_projection[j] /= norm_value;
    }
}

float lda(const std::vector<float>& input) {
    if (input.size() != lda_projection.size()) {
        throw std::runtime_error("Invalid input data");
    }

    float result = 0;

    for (size_t j = 0; j < input.size(); j++) {
        result += input[j] * lda_projection[j];
    }

    return result;
}

int main() {
    lda_fit(X, Y);
    float projection = lda({ 4, 5 });

    return projection >= 0 ? 1 : 0;
}
