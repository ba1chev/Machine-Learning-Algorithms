#include <vector>
#include <stdexcept>
#include <limits>

std::vector<int> hierarchical_clustering_labels;
std::vector<std::vector<float>> X = { { 1, 1 }, { 1, 2 }, { 2, 1 }, { 8, 8 }, { 8, 9 }, { 9, 8 } };
size_t count_of_clusters = 2;

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

float single_linkage(const std::vector<std::vector<float>>& input, const std::vector<size_t>& left_cluster,
    const std::vector<size_t>& right_cluster) {
    float result = std::numeric_limits<float>::max();

    for (size_t i : left_cluster) {
        for (size_t j : right_cluster) {
            float distance = squared_distance(input[i], input[j]);

            if (distance < result) {
                result = distance;
            }
        }
    }

    return result;
}

void find_closest_pair(const std::vector<std::vector<float>>& input,
    const std::vector<std::vector<size_t>>& clusters, size_t& best_left, size_t& best_right) {
    float best_distance = std::numeric_limits<float>::max();

    for (size_t i = 0; i < clusters.size(); i++) {
        for (size_t j = i + 1; j < clusters.size(); j++) {
            float distance = single_linkage(input, clusters[i], clusters[j]);

            if (distance < best_distance) {
                best_distance = distance;
                best_left = i;
                best_right = j;
            }
        }
    }
}

std::vector<int> hierarchical_clustering_fit(const std::vector<std::vector<float>>& input) {
    if (input.size() < count_of_clusters) {
        throw std::runtime_error("Invalid input data");
    }

    std::vector<std::vector<size_t>> clusters(input.size());

    for (size_t i = 0; i < input.size(); i++) {
        clusters[i].push_back(i);
    }

    while (clusters.size() > count_of_clusters) {
        size_t best_left = 0;
        size_t best_right = 0;
        find_closest_pair(input, clusters, best_left, best_right);

        for (size_t i : clusters[best_right]) {
            clusters[best_left].push_back(i);
        }

        clusters.erase(clusters.begin() + best_right);
    }

    std::vector<int> result(input.size(), 0);

    for (size_t i = 0; i < clusters.size(); i++) {
        for (size_t j : clusters[i]) {
            result[j] = (int)i;
        }
    }

    return result;
}

int hierarchical_clustering(size_t point_index) {
    if (hierarchical_clustering_labels.empty()) {
        throw std::runtime_error("Model is not trained");
    }

    if (point_index >= hierarchical_clustering_labels.size()) {
        throw std::runtime_error("Invalid input data");
    }

    return hierarchical_clustering_labels[point_index];
}

int main() {
    hierarchical_clustering_labels = hierarchical_clustering_fit(X);

    return 0;
}
