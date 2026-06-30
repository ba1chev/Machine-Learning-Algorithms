#include <cmath>
#include <vector>
#include <stdexcept>
#include <limits>

struct Node {
public:
    bool is_leaf = false;
    float prediction = 0;
    size_t feature_index = 0;
    float threshold = 0;
    int left = -1;
    int right = -1;
};

std::vector<std::vector<Node>> gradient_boosting_trees;
float gradient_boosting_initial_prediction = 0;

std::vector<std::vector<float>> X = { { 2, 3 }, { 1, 1 }, { 3, 4 }, { 5, 6 }, { 6, 5 }, { 7, 8 } };
std::vector<float> Y = { 0, 0, 0, 1, 1, 1 };
size_t count_of_trees = 20;
size_t max_depth = 3;
size_t min_samples_split = 2;
float learning_rate = 0.1f;

float sigmoid(float input) {
    return 1.0f / (1.0f + std::exp(-input));
}

float mean_value(const std::vector<float>& values, const std::vector<size_t>& indices) {
    if (indices.empty()) {
        return 0;
    }

    float result = 0;

    for (size_t i : indices) {
        result += values[i];
    }

    return result / indices.size();
}

float variance(const std::vector<float>& values, const std::vector<size_t>& indices) {
    if (indices.empty()) {
        return 0;
    }

    float current_mean = mean_value(values, indices);
    float result = 0;

    for (size_t i : indices) {
        float diff_value = values[i] - current_mean;
        result += diff_value * diff_value;
    }

    return result / indices.size();
}

void find_best_split(const std::vector<std::vector<float>>& input, const std::vector<float>& targets,
    const std::vector<size_t>& indices, size_t& best_feature, float& best_threshold, float& best_score) {
    size_t count_of_features = input[0].size();
    best_score = std::numeric_limits<float>::max();

    for (size_t feature = 0; feature < count_of_features; feature++) {
        for (size_t i : indices) {
            float threshold = input[i][feature];
            std::vector<size_t> left_indices;
            std::vector<size_t> right_indices;

            for (size_t j : indices) {
                if (input[j][feature] <= threshold) {
                    left_indices.push_back(j);
                } else {
                    right_indices.push_back(j);
                }
            }

            if (left_indices.empty() || right_indices.empty()) {
                continue;
            }

            float left_weight = (float)left_indices.size() / indices.size();
            float right_weight = (float)right_indices.size() / indices.size();
            float score = left_weight * variance(targets, left_indices) +
                right_weight * variance(targets, right_indices);

            if (score < best_score) {
                best_score = score;
                best_feature = feature;
                best_threshold = threshold;
            }
        }
    }
}

int build_tree(std::vector<Node>& tree, const std::vector<std::vector<float>>& input,
    const std::vector<float>& targets, const std::vector<size_t>& indices, size_t depth = 0) {
    Node node;
    int node_index = tree.size();
    tree.push_back(node);

    float current_variance = variance(targets, indices);

    if (depth >= max_depth || indices.size() < min_samples_split || current_variance == 0) {
        tree[node_index].is_leaf = true;
        tree[node_index].prediction = mean_value(targets, indices);
        return node_index;
    }

    size_t best_feature = 0;
    float best_threshold = 0;
    float best_score = 0;
    find_best_split(input, targets, indices, best_feature, best_threshold, best_score);

    if (best_score >= current_variance) {
        tree[node_index].is_leaf = true;
        tree[node_index].prediction = mean_value(targets, indices);
        return node_index;
    }

    std::vector<size_t> left_indices;
    std::vector<size_t> right_indices;

    for (size_t i : indices) {
        if (input[i][best_feature] <= best_threshold) {
            left_indices.push_back(i);
        } else {
            right_indices.push_back(i);
        }
    }

    tree[node_index].feature_index = best_feature;
    tree[node_index].threshold = best_threshold;
    tree[node_index].left = build_tree(tree, input, targets, left_indices, depth + 1);
    tree[node_index].right = build_tree(tree, input, targets, right_indices, depth + 1);

    return node_index;
}

float predict_tree(const std::vector<Node>& tree, const std::vector<float>& input) {
    int current_index = 0;

    while (!tree[current_index].is_leaf) {
        const Node& current_node = tree[current_index];

        if (input[current_node.feature_index] <= current_node.threshold) {
            current_index = current_node.left;
        } else {
            current_index = current_node.right;
        }
    }

    return tree[current_index].prediction;
}

float raw_score(const std::vector<float>& input) {
    float result = gradient_boosting_initial_prediction;

    for (const std::vector<Node>& tree : gradient_boosting_trees) {
        result += learning_rate * predict_tree(tree, input);
    }

    return result;
}

void gradient_boosting_fit(const std::vector<std::vector<float>>& input, const std::vector<float>& labels) {
    if (input.empty() || input.size() != labels.size()) {
        throw std::runtime_error("Invalid input data");
    }

    gradient_boosting_trees.clear();
    const float epsilon = 1e-7f;

    float positive_ratio = 0;

    for (float label : labels) {
        positive_ratio += label;
    }

    positive_ratio /= labels.size();

    if (positive_ratio < epsilon) {
        positive_ratio = epsilon;
    }
    if (positive_ratio > 1 - epsilon) {
        positive_ratio = 1 - epsilon;
    }

    gradient_boosting_initial_prediction = std::log(positive_ratio / (1 - positive_ratio));

    std::vector<float> current_scores(input.size(), gradient_boosting_initial_prediction);
    std::vector<size_t> all_indices(input.size());

    for (size_t i = 0; i < input.size(); i++) {
        all_indices[i] = i;
    }

    for (size_t i = 0; i < count_of_trees; i++) {
        std::vector<float> residuals(input.size(), 0);

        for (size_t j = 0; j < input.size(); j++) {
            residuals[j] = labels[j] - sigmoid(current_scores[j]);
        }

        std::vector<Node> tree;
        build_tree(tree, input, residuals, all_indices);

        for (size_t j = 0; j < input.size(); j++) {
            current_scores[j] += learning_rate * predict_tree(tree, input[j]);
        }

        gradient_boosting_trees.push_back(tree);
    }
}

float gradient_boosting(const std::vector<float>& input) {
    if (gradient_boosting_trees.empty()) {
        throw std::runtime_error("Forest is not trained");
    }

    return sigmoid(raw_score(input)) >= 0.5f ? 1 : 0;
}

int main() {
    gradient_boosting_fit(X, Y);
    return static_cast<int>(gradient_boosting({ 4, 5 }));
}
