#pragma once

#include <memory>
#include <vector>

#include "source/data/data_structures/tree/tree.hpp"
#include "source/learning_models/encoding/encoder_template.hpp"
#include "source/learning_models/supervised_learning/classification/decision_tree/decision_node.hpp"

class TreeEncoder: public EncoderTemplate<Tree<DecisionNode>, Matrix<float>> {
private:
    size_t encode_node(const TreeNode<DecisionNode>* node, std::vector<std::vector<float>>& rows_out) const;

public:
    Matrix<float> encode(const Tree<DecisionNode>& tree) const override;
    std::unique_ptr<EncoderTemplate<Tree<DecisionNode>, Matrix<float>>> clone() const override;
    EncoderType get_type() const override;
};
