#pragma once

#include <memory>
#include <string>

#include "source/data/input_output/writers/writer.hpp"

template <class T>
class AbstractWriterFactory {
public:
    virtual std::unique_ptr<Writer<T>> create(WriterType writer_type, const std::string& filename) const = 0;
    virtual ~AbstractWriterFactory() = default;
};
