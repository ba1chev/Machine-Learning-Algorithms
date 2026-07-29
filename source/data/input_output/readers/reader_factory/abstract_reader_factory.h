#pragma once

#include <memory>
#include <string>

#include "source/data/input_output/readers/reader.h"

class AbstractReaderFactory {
public:
    virtual std::unique_ptr<Reader> create(ReaderType reader_type, const std::string& filename) const = 0;
    virtual ~AbstractReaderFactory() = default;
};