#pragma once

#include <ostream>

#include "source/data/input_output/csv/csv_field/csv_field.h"
#include "source/data/data_structures/heterogenous_container/heterogeneous_container.hpp"

struct CSVRow {
public:
    HeterogeneousContainer<CSVField> row_data;

    void add_field(const CSVField* csv_field);
    void remove_field(size_t index);
    size_t get_count_of_fields() const;

    friend std::ostream& operator << (std::ostream& os, const CSVRow& csv_row);
};