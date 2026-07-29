#pragma once

#include <vector>
#include <ostream>

#include "source/data/input_output/csv/csv_row/csv_row.h"

class CSVTable {
private:
    std::vector<CSVRow> table_data;
    size_t count_of_csv_cols = 0;

public:
    CSVTable() = default;

    void add_row(const CSVRow& csv_row);
    void remove_row(size_t index);

    size_t get_count_of_rows() const;
    size_t get_count_of_cols() const;
    const CSVRow& operator [] (size_t index) const;

    friend std::ostream& operator << (std::ostream& os, const CSVTable& csv_table);
};