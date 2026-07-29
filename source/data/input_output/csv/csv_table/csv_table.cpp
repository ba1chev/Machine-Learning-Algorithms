#include <stdexcept>

#include "source/data/input_output/csv/csv_table/csv_table.h"

void CSVTable::add_row(const CSVRow& csv_row) {
    if (this->table_data.empty()) {
        this->count_of_csv_cols = csv_row.get_count_of_fields();
    } else if (csv_row.get_count_of_fields() != this->count_of_csv_cols) {
        throw std::out_of_range("Input count of fields is out of range");
    }

    this->table_data.push_back(csv_row);
}

void CSVTable::remove_row(size_t index) {
    if (index >= this->table_data.size()) {
        throw std::out_of_range("Index is out of range");
    }

    this->table_data.erase(this->table_data.begin() + index);
}

size_t CSVTable::get_count_of_rows() const {
    return this->table_data.size();
}

size_t CSVTable::get_count_of_cols() const {
    return this->count_of_csv_cols;
}

const CSVRow& CSVTable::operator [] (size_t index) const {
    if (index >= this->table_data.size()) {
        throw std::out_of_range("Index is out of range");
    }

    return this->table_data[index];
}

std::ostream& operator << (std::ostream& os, const CSVTable& csv_table) {
    for (const CSVRow& row : csv_table.table_data) {
        os << row << std::endl;
    }

    return os;
}