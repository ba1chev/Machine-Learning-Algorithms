#pragma once

#include <memory>
#include <ostream>

#include "source/data/input_output/csv/csv_field/csv_field.h"
#include "source/data/input_output/csv/csv_field/csv_field_type.h"

struct CSVFieldNUM: public CSVField {
public:
    float field_data = 0;
    
    std::unique_ptr<CSVField> clone() const override;
    CSVFieldType get_type() const override;
    void print(std::ostream& os) const override;
};