#pragma once

#include <memory>
#include <ostream>

#include "source/data/input_output/csv/csv_field/csv_field_type.h"

struct CSVField {
public:
    virtual std::unique_ptr<CSVField> clone() const = 0;
    virtual CSVFieldType get_type() const = 0;
    virtual void print(std::ostream& os) const = 0;
    virtual ~CSVField() = default;
};