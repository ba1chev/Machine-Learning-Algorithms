#include "source/learning_models/encoding/tree_encoder/tree_encoder.h"

size_t TreeEncoder::encode_node(const TreeNode<DecisionNode>* node, std::vector<std::vector<float>>& rows_out) const {
    size_t my_index = rows_out.size();
    rows_out.push_back({0.f, 0.f, -1.f, -1.f, 0.f, 0.f});

    if (node->value.is_leaf) {
        rows_out[my_index] = {0.f, 0.f, -1.f, -1.f, node->value.value, 1.f};
    } else {
        size_t left_index = this->encode_node(node->left,  rows_out);
        size_t right_index = this->encode_node(node->right, rows_out);

        rows_out[my_index] = {
            (float)node->value.feature_index,
            node->value.threshold,
            (float)left_index,
            (float)right_index,
            0.f,
            0.f
        };
    }

    return my_index;
}

Matrix<float> TreeEncoder::encode(const Tree<DecisionNode>& tree) const {
    if (tree.empty()) return Matrix<float>();

    std::vector<std::vector<float>> rows_out;
    encode_node(tree.get_root(), rows_out);

    Matrix<float> result;
    result.resize_dims(rows_out.size(), 6);

    for (size_t i = 0; i < rows_out.size(); i++) {
        for (size_t j = 0; j < 6; j++) {
            result[i][j] = rows_out[i][j];
        }
    }

    return result;
}

std::unique_ptr<EncoderTemplate<Tree<DecisionNode>, Matrix<float>>> TreeEncoder::clone() const {
    return std::make_unique<TreeEncoder>(*this);
}

EncoderType TreeEncoder::get_type() const {
    return EncoderType::TREE_DATA_ENCODER;
}
