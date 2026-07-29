#pragma once

#include <memory>

#include "source/data/input_output/csv/csv_field/csv_field.h"
#include "source/data/input_output/csv/csv_field/csv_field_type.h"
#include "source/data/input_output/csv/csv_field/csv_field_factory/abstract_csv_field_factory.h"

class CSVFieldFactory: public AbstractCSVFieldFactory {
public:
    std::unique_ptr<CSVField> create(CSVFieldType field_type) const override;
};
