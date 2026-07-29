#pragma once

#include <memory>
#include <stdexcept>

#include "source/data/data_structures/tree/tree.hpp"
#include "source/data/data_structures/matrix/matrix.hpp"
#include "source/learning_models/decoding/tree_decoder/tree_decoder.h"
#include "source/learning_models/decoding/decoder_factory/abstract_decoder_factory.hpp"
#include "source/learning_models/supervised_learning/classification/decision_tree/decision_node.hpp"

template <class U, class V>
class DecoderFactory: public AbstractDecoderFactory<U, V> {
public:
    std::unique_ptr<DecoderTemplate<U, V>> create(DecoderType decoder_type) const override;
};

template <class U, class V>
std::unique_ptr<DecoderTemplate<U, V>> DecoderFactory<U, V>::create(DecoderType) const {
    throw std::runtime_error("Unsupported decoder type for the requested input/output types");
}

template <>
inline std::unique_ptr<DecoderTemplate<Matrix<float>, Tree<DecisionNode>>>
DecoderFactory<Matrix<float>, Tree<DecisionNode>>::create(DecoderType decoder_type) const {
    switch (decoder_type) {
    case DecoderType::TREE_DATA_DECODER: return std::make_unique<TreeDecoder>();
    default: throw std::runtime_error("Unsupported tree decoder type");
    }
}
