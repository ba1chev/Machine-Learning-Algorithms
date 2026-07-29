#include <memory>
#include <fstream>
#include <sstream>
#include <stdexcept>

#include "source/data/utils.h"
#include "source/data/input_output/readers/csv_reader/csv_reader.h"
#include "source/data/input_output/csv/csv_field/csv_field_num/csv_field_num.h"
#include "source/data/input_output/csv/csv_field/csv_field_str/csv_field_str.h"
#include "source/data/input_output/csv/csv_field/csv_field_factory/csv_field_factory.h"

CSVFieldType CSVReader::classify(const std::string& cell, double& out) const {
    return NUM::is_number(cell, out) ? CSVFieldType::CSV_FIELD_NUM: CSVFieldType::CSV_FIELD_STR;
}

CSVRow CSVReader::parse_line(const std::string& line) {
    CSVRow row;
    std::string cell;
    std::stringstream ss(line);

    while (std::getline(ss, cell, ',')) {
        double number = 0;
        CSVFieldFactory factory;
        
        switch (classify(cell, number)) {
            case CSVFieldType::CSV_FIELD_NUM: {
                std::unique_ptr<CSVField> field = factory.create(CSVFieldType::CSV_FIELD_NUM);
                static_cast<CSVFieldNUM*>(field.get())->field_data = static_cast<float>(number);
                row.add_field(field.get());
                break;
            }
            case CSVFieldType::CSV_FIELD_STR: {
                std::unique_ptr<CSVField> field = factory.create(CSVFieldType::CSV_FIELD_STR);
                static_cast<CSVFieldSTR*>(field.get())->field_data = cell;
                row.add_field(field.get());
                break;
            }
        }
    }

    return row;
}

CSVReader::CSVReader(const std::string& filename) : Reader(filename) {}

size_t CSVReader::read() {
    std::ifstream file(this->filename.c_str());
    if (!file.is_open()) {
        throw std::runtime_error("File is not opened");
    }

    size_t count_of_rows = 0;
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) {
            continue;
        }

        this->csv_table.add_row(parse_line(line));
        count_of_rows += 1;
    }

    file.close();

    return count_of_rows;
}

ReaderType CSVReader::get_type() const {
    return ReaderType::CSV_READER;
}

std::unique_ptr<Reader> CSVReader::clone() const {
    return std::make_unique<CSVReader>(*this);
}

void CSVReader::accept(ReaderVisitor& visitor) const {
    visitor.visit(*this);
}

const CSVTable& CSVReader::get_table() const {
    return this->csv_table;
}
