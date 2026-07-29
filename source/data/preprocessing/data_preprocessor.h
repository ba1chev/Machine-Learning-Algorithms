#pragma once

#include "source/data/input_output/readers/reader.h"
#include "source/data/data_structures/matrix/matrix.hpp"

class DataPreprocessor {
private:
    const Reader* reader = nullptr;

public:
    DataPreprocessor(const Reader* reader);
    Matrix<float> preprocess() const;
};
