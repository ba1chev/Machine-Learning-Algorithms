#pragma once

#include <string>
#include <memory>

#include "source/data/data_structures/matrix/matrix.hpp"
#include "source/data/input_output/writers/writer_type.h"

template <class T>
class Writer {
protected:
    std::string filename;

public:
    Writer(const std::string& filename): filename(filename) {}

    virtual void write(const T& data) const = 0;
    virtual std::unique_ptr<Writer> clone() const = 0;
    virtual WriterType get_type() const = 0;
    virtual ~Writer() = default;
};
