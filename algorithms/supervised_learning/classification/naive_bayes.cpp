#include <cmath>
#include <vector>
#include <stdexcept>

std::vector<std::vector<float>> X = { { 2, 3 }, { 1, 1 }, { 3, 4 }, { 5, 6 }, { 6, 5 }, { 7, 8 } };
std::vector<float> Y = { 0, 0, 0, 1, 1, 1 };

std::vector<float> naive_bayes_means_negative;
std::vector<float> naive_bayes_means_positive;
std::vector<float> naive_bayes_variances_negative;
std::vector<float> naive_bayes_variances_positive;
float naive_bayes_prior_negative = 0;
float naive_bayes_prior_positive = 0;

void compute_class_statistics(const std::vector<std::vector<float>>& input, const std::vector<float>& labels,
    float target_class, std::vector<float>& means, std::vector<float>& variances, float& prior) {
    size_t count_of_features = input[0].size();
    means.assign(count_of_features, 0);
    variances.assign(count_of_features, 0);
    size_t count_of_samples = 0;

    for (size_t i = 0; i < input.size(); i++) {
        if (labels[i] != target_class) {
            continue;
        }

        for (size_t j = 0; j < count_of_features; j++) {
            means[j] += input[i][j];
        }

        count_of_samples++;
    }

    if (count_of_samples == 0) {
        throw std::runtime_error("Invalid input data");
    }

    for (size_t j = 0; j < count_of_features; j++) {
        means[j] /= count_of_samples;
    }

    for (size_t i = 0; i < input.size(); i++) {
        if (labels[i] != target_class) {
            continue;
        }

        for (size_t j = 0; j < count_of_features; j++) {
            float diff_value = input[i][j] - means[j];
            variances[j] += diff_value * diff_value;
        }
    }

    const float epsilon = 1e-7f;

    for (size_t j = 0; j < count_of_features; j++) {
        variances[j] = variances[j] / count_of_samples + epsilon;
    }

    prior = (float)count_of_samples / input.size();
}

void naive_bayes_fit(const std::vector<std::vector<float>>& input, const std::vector<float>& labels) {
    if (input.empty() || input.size() != labels.size()) {
        throw std::runtime_error("Invalid input data");
    }

    compute_class_statistics(input, labels, 0, naive_bayes_means_negative, naive_bayes_variances_negative,
        naive_bayes_prior_negative);
    compute_class_statistics(input, labels, 1, naive_bayes_means_positive, naive_bayes_variances_positive,
        naive_bayes_prior_positive);
}

float log_likelihood(const std::vector<float>& input, const std::vector<float>& means,
    const std::vector<float>& variances, float prior) {
    float result = std::log(prior);

    for (size_t i = 0; i < input.size(); i++) {
        float diff_value = input[i] - means[i];
        result -= 0.5f * std::log(2 * M_PI * variances[i]);
        result -= 0.5f * diff_value * diff_value / variances[i];
    }

    return result;
}

float naive_bayes(const std::vector<float>& input) {
    if (input.size() != naive_bayes_means_negative.size()) {
        throw std::runtime_error("Invalid input data");
    }

    float score_negative = log_likelihood(input, naive_bayes_means_negative, naive_bayes_variances_negative,
        naive_bayes_prior_negative);
    float score_positive = log_likelihood(input, naive_bayes_means_positive, naive_bayes_variances_positive,
        naive_bayes_prior_positive);

    return score_positive >= score_negative ? 1 : 0;
}

int main() {
    naive_bayes_fit(X, Y);
    return static_cast<int>(naive_bayes({ 4, 5 }));
}
