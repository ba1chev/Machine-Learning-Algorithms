#pragma once

#include <memory>

#include "source/data/input_output/csv/csv_field/csv_field.h"
#include "source/data/input_output/csv/csv_field/csv_field_type.h"

class AbstractCSVFieldFactory {
public:
    virtual std::unique_ptr<CSVField> create(CSVFieldType field_type) const = 0;
    virtual ~AbstractCSVFieldFactory() = default;
};
