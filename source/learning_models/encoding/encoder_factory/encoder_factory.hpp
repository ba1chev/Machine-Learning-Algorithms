#pragma once

#include <memory>
#include <string>
#include <stdexcept>

#include "source/data/data_structures/tree/tree.hpp"
#include "source/data/data_structures/matrix/matrix.hpp"
#include "source/learning_models/encoding/tree_encoder/tree_encoder.h"
#include "source/learning_models/encoding/one_hot_encoder/one_hot_encoder.h"
#include "source/learning_models/encoding/ordinal_encoder/ordinal_encoder.h"
#include "source/learning_models/encoding/encoder_factory/abstract_encoder_factory.hpp"
#include "source/learning_models/supervised_learning/classification/decision_tree/decision_node.hpp"

template <class U, class V>
class EncoderFactory: public AbstractEncoderFactory<U, V> {
public:
    std::unique_ptr<EncoderTemplate<U, V>> create(EncoderType encoder_type) const override;
};

template <class U, class V>
std::unique_ptr<EncoderTemplate<U, V>> EncoderFactory<U, V>::create(EncoderType) const {
    throw std::runtime_error("Unsupported encoder type for the requested input/output types");
}

template <>
inline std::unique_ptr<EncoderTemplate<Matrix<std::string>, Matrix<float>>>
EncoderFactory<Matrix<std::string>, Matrix<float>>::create(EncoderType encoder_type) const {
    switch (encoder_type) {
    case EncoderType::ONE_HOT_ENCODER: return std::make_unique<OneHotEncoder>();
    case EncoderType::ORDINAL_ENCODER: return std::make_unique<OrdinalEncoder>();
    default: throw std::runtime_error("Unsupported categorical encoder type");
    }
}

template <>
inline std::unique_ptr<EncoderTemplate<Tree<DecisionNode>, Matrix<float>>>
EncoderFactory<Tree<DecisionNode>, Matrix<float>>::create(EncoderType encoder_type) const {
    switch (encoder_type) {
    case EncoderType::TREE_DATA_ENCODER: return std::make_unique<TreeEncoder>();
    default: throw std::runtime_error("Unsupported tree encoder type");
    }
}
