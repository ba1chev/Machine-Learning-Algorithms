#include <stdexcept>

#include "source/learning_models/decoding/tree_decoder/tree_decoder.h"

TreeNode<DecisionNode>* TreeDecoder::decode_node(const Matrix<float>& m, size_t index) const {
    if (index >= m.get_rows_count()) {
        throw std::runtime_error("TreeDecoder: node index out of range");
    }

    DecisionNode payload;
    if (m[index][5] != 0.f) {
        payload.is_leaf = true;
        payload.value = m[index][4];
    } else {
        payload.is_leaf = false;
        payload.feature_index = (size_t)m[index][0];
        payload.threshold = m[index][1];
    }

    Tree<DecisionNode> helper;
    TreeNode<DecisionNode>* node = helper.create_node(payload);

    if (!payload.is_leaf) {
        node->left = this->decode_node(m, (size_t)m[index][2]);
        node->right = this->decode_node(m, (size_t)m[index][3]);
    }

    return node;
}

Tree<DecisionNode> TreeDecoder::decode(const Matrix<float>& data) const {
    Tree<DecisionNode> tree;

    if (data.get_rows_count() == 0) return tree;

    tree.set_root(decode_node(data, 0));
    return tree;
}

std::unique_ptr<DecoderTemplate<Matrix<float>, Tree<DecisionNode>>> TreeDecoder::clone() const {
    return std::make_unique<TreeDecoder>(*this);
}

DecoderType TreeDecoder::get_type() const {
    return DecoderType::TREE_DATA_DECODER;
}
