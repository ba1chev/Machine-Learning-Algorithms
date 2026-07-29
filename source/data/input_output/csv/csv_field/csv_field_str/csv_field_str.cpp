#include "source/data/input_output/csv/csv_field/csv_field_str/csv_field_str.h"

std::unique_ptr<CSVField> CSVFieldSTR::clone() const {
    return std::make_unique<CSVFieldSTR>(*this);
}

void CSVFieldSTR::print(std::ostream& os) const {
    os << this->field_data;
}

CSVFieldType CSVFieldSTR::get_type() const {
    return CSVFieldType::CSV_FIELD_STR;
}