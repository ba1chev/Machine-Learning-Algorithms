#include <stdexcept>

#include "source/data/preprocessing/data_preprocessor.h"
#include "source/data/preprocessing/matrix_building_visitor/matrix_building_visitor.h"

DataPreprocessor::DataPreprocessor(const Reader* reader) {
    if (!reader) {
        throw std::runtime_error("Nullptr detected");
    }

    this->reader = reader;
}

Matrix<float> DataPreprocessor::preprocess() const {
    MatrixBuildingVisitor visitor;
    this->reader->accept(visitor);
    return visitor.result();
}
