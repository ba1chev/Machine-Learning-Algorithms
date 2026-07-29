#pragma once

#include <memory>
#include <string>
#include <ostream>

#include "source/data/input_output/csv/csv_field/csv_field.h"
#include "source/data/input_output/csv/csv_field/csv_field_type.h"

struct CSVFieldSTR: public CSVField {
public:
    std::string field_data;
    std::unique_ptr<CSVField> clone() const override;
    CSVFieldType get_type() const override;
    void print(std::ostream& os) const override;
};