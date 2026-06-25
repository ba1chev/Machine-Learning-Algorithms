#include <vector>
#include <stdexcept>

const int NOISE = -1;
const int UNVISITED = -2;

std::vector<int> dbscan_labels;
std::vector<std::vector<float>> X = { { 1, 1 }, { 1, 2 }, { 2, 1 }, { 8, 8 }, { 8, 9 }, { 9, 8 }, { 50, 50 } };
float epsilon = 2.0f;
size_t min_samples = 2;

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

std::vector<size_t> find_neighbors(const std::vector<std::vector<float>>& input, size_t point_index) {
    std::vector<size_t> result;
    float epsilon_squared = epsilon * epsilon;

    for (size_t i = 0; i < input.size(); i++) {
        if (squared_distance(input[point_index], input[i]) <= epsilon_squared) {
            result.push_back(i);
        }
    }

    return result;
}

void expand_cluster(const std::vector<std::vector<float>>& input, std::vector<int>& labels,
    size_t point_index, std::vector<size_t>& neighbors, int cluster_id) {
    labels[point_index] = cluster_id;

    for (size_t i = 0; i < neighbors.size(); i++) {
        size_t current = neighbors[i];

        if (labels[current] == NOISE) {
            labels[current] = cluster_id;
        }

        if (labels[current] != UNVISITED) {
            continue;
        }

        labels[current] = cluster_id;
        std::vector<size_t> current_neighbors = find_neighbors(input, current);

        if (current_neighbors.size() >= min_samples) {
            for (size_t next : current_neighbors) {
                neighbors.push_back(next);
            }
        }
    }
}

std::vector<int> dbscan_fit(const std::vector<std::vector<float>>& input) {
    std::vector<int> result(input.size(), UNVISITED);
    int cluster_id = 0;

    for (size_t i = 0; i < input.size(); i++) {
        if (result[i] != UNVISITED) {
            continue;
        }

        std::vector<size_t> neighbors = find_neighbors(input, i);

        if (neighbors.size() < min_samples) {
            result[i] = NOISE;
            continue;
        }

        expand_cluster(input, result, i, neighbors, cluster_id);
        cluster_id++;
    }

    return result;
}

int dbscan(size_t point_index) {
    if (dbscan_labels.empty()) {
        throw std::runtime_error("Model is not trained");
    }

    if (point_index >= dbscan_labels.size()) {
        throw std::runtime_error("Invalid input data");
    }

    return dbscan_labels[point_index];
}

int main() {
    dbscan_labels = dbscan_fit(X);

    return 0;
}
