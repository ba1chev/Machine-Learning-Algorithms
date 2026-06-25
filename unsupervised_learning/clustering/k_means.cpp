#include <vector>
#include <stdexcept>
#include <limits>

std::vector<std::vector<float>> k_means_centroids;
std::vector<std::vector<float>> X = { { 1, 1 }, { 1, 2 }, { 2, 1 }, { 8, 8 }, { 8, 9 }, { 9, 8 } };
size_t count_of_clusters = 2;
size_t max_iterations = 100;

float squared_distance(const std::vector<float>& left, const std::vector<float>& right) {
    if (left.size() != right.size()) {
        throw std::runtime_error("Invalid input data");
    }

    float result = 0;

    for (size_t i = 0; i < left.size(); i++) {
        float diff_value = left[i] - right[i];
        result += diff_value * diff_value;
    }

    return result;
}

size_t closest_centroid(const std::vector<float>& sample, const std::vector<std::vector<float>>& centroids) {
    size_t result = 0;
    float best_distance = std::numeric_limits<float>::max();

    for (size_t i = 0; i < centroids.size(); i++) {
        float distance = squared_distance(sample, centroids[i]);

        if (distance < best_distance) {
            best_distance = distance;
            result = i;
        }
    }

    return result;
}

std::vector<size_t> assign_clusters(const std::vector<std::vector<float>>& input,
    const std::vector<std::vector<float>>& centroids) {
    std::vector<size_t> result(input.size(), 0);

    for (size_t i = 0; i < input.size(); i++) {
        result[i] = closest_centroid(input[i], centroids);
    }

    return result;
}

std::vector<std::vector<float>> update_centroids(const std::vector<std::vector<float>>& input,
    const std::vector<size_t>& assignments, size_t count_of_features) {
    std::vector<std::vector<float>> result(count_of_clusters, std::vector<float>(count_of_features, 0));
    std::vector<size_t> counts(count_of_clusters, 0);

    for (size_t i = 0; i < input.size(); i++) {
        size_t cluster = assignments[i];

        for (size_t j = 0; j < count_of_features; j++) {
            result[cluster][j] += input[i][j];
        }

        counts[cluster]++;
    }

    for (size_t i = 0; i < count_of_clusters; i++) {
        if (counts[i] == 0) {
            continue;
        }

        for (size_t j = 0; j < count_of_features; j++) {
            result[i][j] /= counts[i];
        }
    }

    return result;
}

std::vector<std::vector<float>> k_means_fit(const std::vector<std::vector<float>>& input) {
    if (input.empty() || input.size() < count_of_clusters) {
        throw std::runtime_error("Invalid input data");
    }

    size_t count_of_features = input[0].size();
    std::vector<std::vector<float>> centroids(count_of_clusters, std::vector<float>(count_of_features, 0));

    for (size_t i = 0; i < count_of_clusters; i++) {
        centroids[i] = input[i];
    }

    for (size_t i = 0; i < max_iterations; i++) {
        std::vector<size_t> assignments = assign_clusters(input, centroids);
        std::vector<std::vector<float>> next_centroids = update_centroids(input, assignments, count_of_features);

        float total_shift = 0;

        for (size_t j = 0; j < count_of_clusters; j++) {
            total_shift += squared_distance(centroids[j], next_centroids[j]);
        }

        centroids = next_centroids;

        if (total_shift == 0) {
            break;
        }
    }

    return centroids;
}

size_t k_means(const std::vector<float>& input) {
    if (k_means_centroids.empty()) {
        throw std::runtime_error("Model is not trained");
    }

    return closest_centroid(input, k_means_centroids);
}

int main() {
    k_means_centroids = k_means_fit(X);

    return 0;
}
