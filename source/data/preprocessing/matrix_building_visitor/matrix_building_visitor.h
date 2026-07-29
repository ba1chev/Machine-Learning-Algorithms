#pragma once

#include "source/data/data_structures/matrix/matrix.hpp"
#include "source/data/input_output/readers/reader_visitor.h"

class MatrixBuildingVisitor: public ReaderVisitor {
private:
    Matrix<float> matrix;

public:
    void visit(const CSVReader& csv_reader) override;
    void visit(const BINReader<float>& bin_reader) override;
    const Matrix<float>& result() const;
};
