#include <stdexcept>

#include "source/data/input_output/readers/csv_reader/csv_reader.h"
#include "source/data/input_output/readers/bin_reader/bin_reader.hpp"
#include "source/data/input_output/csv/csv_field/csv_field_num/csv_field_num.h"
#include "source/data/preprocessing/matrix_building_visitor/matrix_building_visitor.h"

void MatrixBuildingVisitor::visit(const CSVReader& csv_reader) {
    const CSVTable& table = csv_reader.get_table();
    size_t count_of_rows = table.get_count_of_rows();
    size_t count_of_cols = table.get_count_of_cols();

    this->matrix.resize_dims(count_of_rows, count_of_cols);

    for (size_t i = 0; i < count_of_rows; i++) {
        const CSVRow& row = table[i];
        for (size_t j = 0; j < count_of_cols; j++) {
            const CSVField* field = row.row_data[j];

            if (field->get_type() != CSVFieldType::CSV_FIELD_NUM) {
                throw std::runtime_error("String columns not yet supported");
            }

            this->matrix[i][j] = static_cast<const CSVFieldNUM*>(field)->field_data;
        }
    }
}

void MatrixBuildingVisitor::visit(const BINReader<float>& bin_reader) {
    const BINFile<float>& file = bin_reader.get_file();
    size_t count_of_rows = file.get_count_of_records();
    size_t count_of_cols = file.get_count_of_values();

    this->matrix.resize_dims(count_of_rows, count_of_cols);

    for (size_t i = 0; i < count_of_rows; i++) {
        const BINRecord<float>& record = file[i];
        for (size_t j = 0; j < count_of_cols; j++) {
            this->matrix[i][j] = record.record_data[j].value;
        }
    }
}

const Matrix<float>& MatrixBuildingVisitor::result() const {
    return this->matrix;
}
