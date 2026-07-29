#pragma once

#include <memory>

#include "source/data/input_output/readers/reader.h"
#include "source/data/input_output/readers/reader_factory/abstract_reader_factory.h"

class ReaderFactory: public AbstractReaderFactory {
public:
    std::unique_ptr<Reader> create(ReaderType reader_type, const std::string& filename) const override;
};