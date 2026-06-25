#include <vector>
#include <stdexcept>

std::vector<std::vector<float>> X = { { 1, 2, 3 }, { 2, 3, 4 }, { 3, 4, 5 }, { 5, 6, 7 }, { 6, 7, 8 } };
std::vector<float> Y = { 0, 0, 0, 1, 1 };
size_t count_of_neighbors = 3;

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

std::vector<size_t> find_nearest_neighbors(const std::vector<float>& input) {
    std::vector<float> distances(X.size(), 0);

    for (size_t i = 0; i < X.size(); i++) {
        distances[i] = squared_distance(input, X[i]);
    }

    std::vector<size_t> result;
    std::vector<bool> used(X.size(), false);

    for (size_t i = 0; i < count_of_neighbors && i < X.size(); i++) {
        size_t best_index = 0;
        float best_distance = 0;
        bool initialized = false;

        for (size_t j = 0; j < X.size(); j++) {
            if (used[j]) {
                continue;
            }

            if (!initialized || distances[j] < best_distance) {
                best_distance = distances[j];
                best_index = j;
                initialized = true;
            }
        }

        used[best_index] = true;
        result.push_back(best_index);
    }

    return result;
}

float knn(const std::vector<float>& input) {
    if (X.empty() || X.size() != Y.size()) {
        throw std::runtime_error("Invalid input data");
    }

    std::vector<size_t> neighbors = find_nearest_neighbors(input);
    float count_of_positives = 0;

    for (size_t i : neighbors) {
        count_of_positives += Y[i];
    }

    return count_of_positives * 2 >= neighbors.size() ? 1 : 0;
}

int main() {

    return 0;
}
