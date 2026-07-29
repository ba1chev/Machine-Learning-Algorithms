#pragma once

#include <memory>

#include "source/data/data_structures/tree/tree.hpp"
#include "source/learning_models/decoding/decoder_template.hpp"
#include "source/learning_models/supervised_learning/classification/decision_tree/decision_node.hpp"

class TreeDecoder: public DecoderTemplate<Matrix<float>, Tree<DecisionNode>> {
private:
    TreeNode<DecisionNode>* decode_node(const Matrix<float>& m, size_t index) const;

public:
    Tree<DecisionNode> decode(const Matrix<float>& data) const override;
    std::unique_ptr<DecoderTemplate<Matrix<float>, Tree<DecisionNode>>> clone() const override;
    DecoderType get_type() const override;
};
