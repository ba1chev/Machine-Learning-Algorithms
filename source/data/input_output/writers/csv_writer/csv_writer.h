#pragma once

#include <string>

#include "source/data/input_output/writers/writer.hpp"
#include "source/data/input_output/csv/csv_table/csv_table.h"

class CSVWriter: public Writer<CSVTable> {
public:
    CSVWriter(const std::string& filename);
    
    void write(const CSVTable& data) const override;
    std::unique_ptr<Writer<CSVTable>> clone() const override;
    WriterType get_type() const override;
};
