#include "source/data/input_output/csv/csv_row/csv_row.h"

#include <stdexcept>

void CSVRow::add_field(const CSVField* csv_field) {
    if (!csv_field) {
        throw std::runtime_error("Nullptr detected");
    }

    this->row_data.add_element(csv_field);
}

void CSVRow::remove_field(size_t index) {
    this->row_data.remove_element(index);
}

size_t CSVRow::get_count_of_fields() const {
    return this->row_data.get_size();
}

std::ostream& operator << (std::ostream& os, const CSVRow& csv_row) {
    size_t count_of_fields = csv_row.row_data.get_size();
    for (size_t i = 0; i < count_of_fields; i++) {
        if (i > 0) {
            os << ",";
        }
        csv_row.row_data[i]->print(os);
    }

    return os;
}