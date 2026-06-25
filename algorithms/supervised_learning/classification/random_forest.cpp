#include <vector>
#include <stdexcept>
#include <limits>
#include <cstdlib>

struct Node {
public:
    bool is_leaf = false;
    float prediction = 0;
    size_t feature_index = 0;
    float threshold = 0;
    int left = -1;
    int right = -1;
};

std::vector<std::vector<Node>> random_forest_trees;
std::vector<std::vector<float>> X = { { 2, 3 }, { 1, 1 }, { 3, 4 }, { 5, 6 }, { 6, 5 }, { 7, 8 } };
std::vector<float> Y = { 0, 0, 0, 1, 1, 1 };
size_t count_of_trees = 10;
size_t max_depth = 5;
size_t min_samples_split = 2;
size_t count_of_features_per_split = 1;

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

std::vector<size_t> pick_random_features(size_t count_of_features) {
    std::vector<size_t> all_features(count_of_features);

    for (size_t i = 0; i < count_of_features; i++) {
        all_features[i] = i;
    }

    std::vector<size_t> result;

    for (size_t i = 0; i < count_of_features_per_split && !all_features.empty(); i++) {
        size_t pick = std::rand() % all_features.size();
        result.push_back(all_features[pick]);
        all_features.erase(all_features.begin() + pick);
    }

    return result;
}

void find_best_split(const std::vector<std::vector<float>>& input, const std::vector<float>& labels,
    const std::vector<size_t>& indices, size_t& best_feature, float& best_threshold, float& best_score) {
    std::vector<size_t> candidate_features = pick_random_features(input[0].size());
    best_score = std::numeric_limits<float>::max();

    for (size_t feature : candidate_features) {
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

int build_tree(std::vector<Node>& tree, const std::vector<std::vector<float>>& input,
    const std::vector<float>& labels, const std::vector<size_t>& indices, size_t depth = 0) {
    Node node;
    int node_index = tree.size();
    tree.push_back(node);

    float current_impurity = gini_impurity(labels, indices);

    if (depth >= max_depth || indices.size() < min_samples_split || current_impurity == 0) {
        tree[node_index].is_leaf = true;
        tree[node_index].prediction = majority_class(labels, indices);
        return node_index;
    }

    size_t best_feature = 0;
    float best_threshold = 0;
    float best_score = 0;
    find_best_split(input, labels, indices, best_feature, best_threshold, best_score);

    if (best_score >= current_impurity) {
        tree[node_index].is_leaf = true;
        tree[node_index].prediction = majority_class(labels, indices);
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
    tree[node_index].left = build_tree(tree, input, labels, left_indices, depth + 1);
    tree[node_index].right = build_tree(tree, input, labels, right_indices, depth + 1);

    return node_index;
}

std::vector<size_t> bootstrap_sample(size_t count_of_samples) {
    std::vector<size_t> result(count_of_samples);

    for (size_t i = 0; i < count_of_samples; i++) {
        result[i] = std::rand() % count_of_samples;
    }

    return result;
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

float random_forest(const std::vector<float>& input) {
    if (random_forest_trees.empty()) {
        throw std::runtime_error("Forest is not trained");
    }

    float count_of_positives = 0;

    for (const std::vector<Node>& tree : random_forest_trees) {
        count_of_positives += predict_tree(tree, input);
    }

    return count_of_positives * 2 >= random_forest_trees.size() ? 1 : 0;
}

int main() {
    for (size_t i = 0; i < count_of_trees; i++) {
        std::vector<size_t> sample_indices = bootstrap_sample(X.size());
        std::vector<Node> tree;
        build_tree(tree, X, Y, sample_indices);
        random_forest_trees.push_back(tree);
    }

    return 0;
}
