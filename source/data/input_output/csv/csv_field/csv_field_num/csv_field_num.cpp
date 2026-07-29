#include "source/data/input_output/csv/csv_field/csv_field_num/csv_field_num.h"

std::unique_ptr<CSVField> CSVFieldNUM::clone() const {
    return std::make_unique<CSVFieldNUM>(*this);
}

void CSVFieldNUM::print(std::ostream& os) const {
    os << this->field_data;
}

CSVFieldType CSVFieldNUM::get_type() const {
    return CSVFieldType::CSV_FIELD_NUM;
}