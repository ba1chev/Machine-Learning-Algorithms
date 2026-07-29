#pragma once

#include <string>
#include <memory>

#include "source/data/input_output/readers/reader_type.h"
#include "source/data/input_output/readers/reader_visitor.h"

class Reader {
protected:
    std::string filename;

public:
    Reader(const std::string& filename);
    
    virtual size_t read() = 0;
    virtual std::unique_ptr<Reader> clone() const = 0;
    virtual ReaderType get_type() const = 0;
    virtual void accept(ReaderVisitor& visitor) const = 0;
    virtual ~Reader() = default;
};