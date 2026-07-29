#pragma once

#include <string>
#include <memory>

#include "source/data/input_output/readers/reader.h"
#include "source/data/input_output/csv/csv_row/csv_row.h"
#include "source/data/input_output/csv/csv_table/csv_table.h"
#include "source/data/input_output/csv/csv_field/csv_field_type.h"

class CSVReader: public Reader {
private:
    CSVTable csv_table;

    CSVFieldType classify(const std::string& cell, double& out) const;
    CSVRow parse_line(const std::string& line);

public:
    CSVReader(const std::string& filename);
    
    size_t read() override;
    std::unique_ptr<Reader> clone() const override;
    ReaderType get_type() const override;
    void accept(ReaderVisitor& visitor) const override;
    const CSVTable& get_table() const;
};
