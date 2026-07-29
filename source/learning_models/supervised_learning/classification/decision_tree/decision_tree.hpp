#pragma once

#include <memory>
#include <vector>
#include <algorithm>
#include <stdexcept>

#include "source/data/data_structures/tree/tree.hpp"
#include "source/learning_models/learning_model_type.h"
#include "source/learning_models/decoding/decoder_template.hpp"
#include "source/learning_models/encoding/encoder_template.hpp"
#include "source/learning_models/supervised_learning/classification/utils.hpp"
#include "source/learning_models/supervised_learning/supervised_learning_model.hpp"
#include "source/learning_models/supervised_learning/classification/decision_tree/decision_node.hpp"

template <class T>
class DecisionTree: public SupervisedLearningModel<T> {
private:
    size_t max_depth;
    Tree<DecisionNode> tree;
    std::shared_ptr<const EncoderTemplate<Tree<DecisionNode>, Matrix<float>>> encoder;
    std::shared_ptr<const DecoderTemplate<Matrix<float>, Tree<DecisionNode>>> decoder;

    bool best_split(const std::vector<size_t>& rows, const Matrix<T>& X, const Matrix<T>& y,
        size_t& out_feature, float& out_threshold, std::vector<size_t>& out_left, std::vector<size_t>& out_right) const;

    TreeNode<DecisionNode>* build(const std::vector<size_t>& rows,
        const Matrix<T>& X, const Matrix<T>& y, size_t depth) const;

public:
    DecisionTree(const Matrix<T>* X, const Matrix<T>* y, size_t max_depth,
        const EncoderTemplate<Tree<DecisionNode>, Matrix<float>>* encoder,
        const DecoderTemplate<Matrix<float>, Tree<DecisionNode>>* decoder);

    DecisionTree(const Matrix<T>* X, const Matrix<T>* y, size_t max_depth,
        std::shared_ptr<const EncoderTemplate<Tree<DecisionNode>, Matrix<float>>> encoder,
        std::shared_ptr<const DecoderTemplate<Matrix<float>, Tree<DecisionNode>>> decoder);

    void fit() override;
    Matrix<T> predict(const Matrix<T>& X) const override;
    LearningModelType get_type() const override;
    std::unique_ptr<LearningModel<T>> clone() const override;

    const Matrix<T>& get_weights() const override;
    void set_weights(const Matrix<T>& m) override;
};

template <class T>
bool DecisionTree<T>::best_split(const std::vector<size_t>& rows, const Matrix<T>& X, const Matrix<T>& y,
    size_t& out_feature, float& out_threshold,
    std::vector<size_t>& out_left, std::vector<size_t>& out_right) const {
    float parent_gini = DECISION_TREE::gini(rows, y);
    float best_gain = 0.f;
    bool found = false;
    size_t n_features = X.get_cols_count();
    float total = (float)rows.size();

    for (size_t feature_index = 0; feature_index < n_features; feature_index++) {
        std::vector<float> values;
        values.reserve(rows.size());

        for (size_t i : rows) {
            values.push_back((float)X[i][feature_index]);
        }

        std::sort(values.begin(), values.end());
        values.erase(std::unique(values.begin(), values.end()), values.end());

        for (size_t value_index = 0; value_index + 1 < values.size(); value_index++) {
            float threshold = (values[value_index] + values[value_index + 1]) / 2.f;
            std::vector<size_t> left_rows, right_rows;

            for (size_t i : rows) {
                if ((float)X[i][feature_index] <= threshold) {
                    left_rows.push_back(i);
                } else {
                    right_rows.push_back(i);
                }
            }
            if (left_rows.empty() || right_rows.empty()) continue;

            float weighted = (left_rows.size() / total) * DECISION_TREE::gini(left_rows, y) +
                (right_rows.size() / total) * DECISION_TREE::gini(right_rows, y);
            float gain = parent_gini - weighted;

            if (gain > best_gain) {
                best_gain = gain;
                found = true;
                out_feature = feature_index;
                out_threshold = threshold;
                out_left = left_rows;
                out_right = right_rows;
            }
        }
    }

    return found;
}

template <class T>
TreeNode<DecisionNode>* DecisionTree<T>::build(const std::vector<size_t>& rows,
    const Matrix<T>& X, const Matrix<T>& y, size_t depth) const {
    DecisionNode payload;

    if (DECISION_TREE::is_pure(rows, y) || depth >= this->max_depth || rows.size() < 2) {
        payload.is_leaf = true;
        payload.value = DECISION_TREE::majority_class(rows, y);
        return this->tree.create_node(payload);
    }

    size_t feat = 0;
    float thresh = 0.f;
    std::vector<size_t> left_rows, right_rows;

    if (!this->best_split(rows, X, y, feat, thresh, left_rows, right_rows)
        || left_rows.empty() || right_rows.empty()) {
        payload.is_leaf = true;
        payload.value = DECISION_TREE::majority_class(rows, y);
        return this->tree.create_node(payload);
    }

    payload.is_leaf = false;
    payload.feature_index = feat;
    payload.threshold = thresh;

    TreeNode<DecisionNode>* node = this->tree.create_node(payload);
    node->left = this->build(left_rows, X, y, depth + 1);
    node->right = this->build(right_rows, X, y, depth + 1);
    return node;
}

template <class T>
DecisionTree<T>::DecisionTree(const Matrix<T>* X, const Matrix<T>* y, size_t max_depth,
    const EncoderTemplate<Tree<DecisionNode>, Matrix<float>>* encoder,
    const DecoderTemplate<Matrix<float>, Tree<DecisionNode>>* decoder):
    SupervisedLearningModel<T>(X, y), max_depth(max_depth),
    encoder(encoder ? encoder->clone() : nullptr),
    decoder(decoder ? decoder->clone() : nullptr) {}

template <class T>
DecisionTree<T>::DecisionTree(const Matrix<T>* X, const Matrix<T>* y, size_t max_depth,
    std::shared_ptr<const EncoderTemplate<Tree<DecisionNode>, Matrix<float>>> encoder,
    std::shared_ptr<const DecoderTemplate<Matrix<float>, Tree<DecisionNode>>> decoder):
    SupervisedLearningModel<T>(X, y), max_depth(max_depth),
    encoder(std::move(encoder)), decoder(std::move(decoder)) {}

template <class T>
void DecisionTree<T>::fit() {
    const Matrix<T>& X = *this->get_X();
    const Matrix<T>& y = *this->get_y();

    size_t n = X.get_rows_count();
    std::vector<size_t> all_rows(n);
    for (size_t i = 0; i < n; i++) {
        all_rows[i] = i;
    }

    TreeNode<DecisionNode>* root = this->build(all_rows, X, y, 0);
    this->tree.set_root(root);

    Matrix<float> encoded = this->encoder->encode(this->tree);

    Matrix<T> weights;
    weights.resize_dims(encoded.get_rows_count(), 6);
    for (size_t i = 0; i < encoded.get_rows_count(); i++) {
        for (size_t j = 0; j < 6; j++) {
            weights[i][j] = (T)encoded[i][j];
        }
    }

    this->weights = weights;
}

template <class T>
Matrix<T> DecisionTree<T>::predict(const Matrix<T>& X) const {
    if (this->tree.empty()) {
        throw std::runtime_error("DecisionTree: model not fitted");
    }

    size_t n = X.get_rows_count();
    Matrix<T> labels;
    labels.resize_dims(n, 1);

    for (size_t i = 0; i < n; i++) {
        const TreeNode<DecisionNode>* node = this->tree.get_root();

        while (node && !node->value.is_leaf) {
            if ((float)X[i][node->value.feature_index] <= node->value.threshold) {
                node = node->left;
            } else {
                node = node->right;
            }
        }

        labels[i][0] = node ? (T)node->value.value : T(0);
    }

    return labels;
}

template <class T>
LearningModelType DecisionTree<T>::get_type() const {
    return LearningModelType::DecisionTree;
}

template <class T>
std::unique_ptr<LearningModel<T>> DecisionTree<T>::clone() const {
    return std::make_unique<DecisionTree<T>>(*this);
}

template <class T>
const Matrix<T>& DecisionTree<T>::get_weights() const {
    return this->weights;
}

template <class T>
void DecisionTree<T>::set_weights(const Matrix<T>& m) {
    this->weights = m;
    this->tree.clear();

    if (m.get_rows_count() > 0) {
        Matrix<float> float_m;
        float_m.resize_dims(m.get_rows_count(), 6);
        
        for (size_t i = 0; i < m.get_rows_count(); i++) {
            for (size_t j = 0; j < 6; j++) {
                float_m[i][j] = (float)m[i][j];
            }
        }
        
        this->tree = this->decoder->decode(float_m);
    }
}
