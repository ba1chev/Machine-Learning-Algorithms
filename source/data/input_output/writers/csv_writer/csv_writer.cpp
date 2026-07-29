#include <fstream>
#include <stdexcept>

#include "source/data/input_output/writers/csv_writer/csv_writer.h"

CSVWriter::CSVWriter(const std::string& filename): Writer(filename) {}

void CSVWriter::write(const CSVTable& data) const {
    std::ofstream os(this->filename);
    if (!os) {
        throw std::runtime_error("Could not open output file for writing");
    }

    os << data;
}

WriterType CSVWriter::get_type() const {
    return WriterType::CSV_WRITER;
}

std::unique_ptr<Writer<CSVTable>> CSVWriter::clone() const {
    return std::make_unique<CSVWriter>(*this);
}
