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

std::vector<Node> decision_tree_nodes;
std::vector<std::vector<float>> X = { { 2, 3 }, { 1, 1 }, { 3, 4 }, { 5, 6 }, { 6, 5 }, { 7, 8 } };
std::vector<float> Y = { 0, 0, 0, 1, 1, 1 };
size_t max_depth = 5;
size_t min_samples_split = 2;

float gini_impurity(const std::vector<float>& labels, const std::vector<size_t>& indices) {
    if (indices.empty()) {
        return 0;
    }

    float count_of_positives = 0;

    for (size_t i : indices) {
        count_of_positives += labels[i];
    }

    float positive_ratio = count_of_positives / indices.size();
    float negative_ratio = 1 - positive_ratio;

    return 1 - positive_ratio * positive_ratio - negative_ratio * negative_ratio;
}

float majority_class(const std::vector<float>& labels, const std::vector<size_t>& indices) {
    float count_of_positives = 0;

    for (size_t i : indices) {
        count_of_positives += labels[i];
    }

    return count_of_positives * 2 >= indices.size() ? 1 : 0;
}

void find_best_split(const std::vector<std::vector<float>>& input, const std::vector<float>& labels,
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
            float score = left_weight * gini_impurity(labels, left_indices) +
                right_weight * gini_impurity(labels, right_indices);

            if (score < best_score) {
                best_score = score;
                best_feature = feature;
                best_threshold = threshold;
            }
        }
    }
}

int build_tree(const std::vector<std::vector<float>>& input, const std::vector<float>& labels,
    const std::vector<size_t>& indices, size_t depth = 0) {
    Node node;
    int node_index = decision_tree_nodes.size();
    decision_tree_nodes.push_back(node);

    float current_impurity = gini_impurity(labels, indices);

    if (depth >= max_depth || indices.size() < min_samples_split || current_impurity == 0) {
        decision_tree_nodes[node_index].is_leaf = true;
        decision_tree_nodes[node_index].prediction = majority_class(labels, indices);
        return node_index;
    }

    size_t best_feature = 0;
    float best_threshold = 0;
    float best_score = 0;
    find_best_split(input, labels, indices, best_feature, best_threshold, best_score);

    if (best_score >= current_impurity) {
        decision_tree_nodes[node_index].is_leaf = true;
        decision_tree_nodes[node_index].prediction = majority_class(labels, indices);
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

    decision_tree_nodes[node_index].feature_index = best_feature;
    decision_tree_nodes[node_index].threshold = best_threshold;
    decision_tree_nodes[node_index].left = build_tree(input, labels, left_indices, depth + 1);
    decision_tree_nodes[node_index].right = build_tree(input, labels, right_indices, depth + 1);

    return node_index;
}

float decision_tree(const std::vector<float>& input) {
    if (decision_tree_nodes.empty()) {
        throw std::runtime_error("Tree is not trained");
    }

    int current_index = 0;

    while (!decision_tree_nodes[current_index].is_leaf) {
        const Node& current_node = decision_tree_nodes[current_index];

        if (input[current_node.feature_index] <= current_node.threshold) {
            current_index = current_node.left;
        } else {
            current_index = current_node.right;
        }
    }

    return decision_tree_nodes[current_index].prediction;
}

int main() {
    std::vector<size_t> indices(X.size());

    for (size_t i = 0; i < X.size(); i++) {
        indices[i] = i;
    }

    build_tree(X, Y, indices);

    return 0;
}
