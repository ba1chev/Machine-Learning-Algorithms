#pragma once

enum class EncoderType {
    // Categorical encoders
    ONE_HOT_ENCODER = (1 << 0),
    ORDINAL_ENCODER = (1 << 1),

    // Data structure module encoders
    TREE_DATA_ENCODER = (1 << 2)
};