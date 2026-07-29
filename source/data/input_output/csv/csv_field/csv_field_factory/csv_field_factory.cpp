#include <stdexcept>

#include "source/data/input_output/csv/csv_field/csv_field_num/csv_field_num.h"
#include "source/data/input_output/csv/csv_field/csv_field_str/csv_field_str.h"
#include "source/data/input_output/csv/csv_field/csv_field_factory/csv_field_factory.h"

std::unique_ptr<CSVField> CSVFieldFactory::create(CSVFieldType field_type) const {
    switch (field_type) {
        case CSVFieldType::CSV_FIELD_NUM: return std::make_unique<CSVFieldNUM>();
        case CSVFieldType::CSV_FIELD_STR: return std::make_unique<CSVFieldSTR>();
        default: throw std::runtime_error("Unsupported CSV field type");
    }
}
