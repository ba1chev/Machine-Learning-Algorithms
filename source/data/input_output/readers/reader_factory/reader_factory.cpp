#include <stdexcept>

#include "source/data/input_output/readers/csv_reader/csv_reader.h"
#include "source/data/input_output/readers/bin_reader/bin_reader.hpp"
#include "source/data/input_output/readers/reader_factory/reader_factory.h"

std::unique_ptr<Reader> ReaderFactory::create(ReaderType reader_type, const std::string& filename) const {
    switch (reader_type) {
        case ReaderType::CSV_READER: return std::make_unique<CSVReader>(filename);
        case ReaderType::BIN_READER: return std::make_unique<BINReader<float>>(filename);
        default: throw std::runtime_error("Not supported reader type");
    }
}
