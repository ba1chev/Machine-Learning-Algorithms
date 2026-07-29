#pragma once

#include <string>
#include <fstream>
#include <stdexcept>

#include "source/data/input_output/readers/reader.h"
#include "source/data/input_output/readers/reader_type.h"
#include "source/data/input_output/readers/reader_visitor.h"
#include "source/data/input_output/bin/bin_file/bin_file.hpp"

template <class T>
class BINReader: public Reader {
private:
    BINFile<T> bin_file;

public:
    BINReader(const std::string& filename);

    size_t read() override;
    std::unique_ptr<Reader> clone() const override;
    ReaderType get_type() const override;
    void accept(ReaderVisitor& visitor) const override;
    const BINFile<T>& get_file() const;
};

template <class T>
BINReader<T>::BINReader(const std::string& filename): Reader(filename) {}

template <class T>
size_t BINReader<T>::read() {
    std::ifstream file(this->filename.c_str(), std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("File is not opened");
    }

    size_t count_of_records = 0;
    size_t values_per_record = 0;
    file.read(reinterpret_cast<char*>(&count_of_records), sizeof(count_of_records));
    file.read(reinterpret_cast<char*>(&values_per_record), sizeof(values_per_record));

    if (!file) {
        throw std::runtime_error("Malformed binary file: could not read header");
    }

    for (size_t i = 0; i < count_of_records; i++) {
        BINRecord<T> record;

        for (size_t j = 0; j < values_per_record; j++) {
            T value = T{};
            file.read(reinterpret_cast<char*>(&value), sizeof(value));

            if (!file) {
                throw std::runtime_error("Malformed binary file: not enough values");
            }

            record.add_value(value);
        }

        this->bin_file.add_record(record);
    }

    file.close();
    return count_of_records;
}

template <class T>
ReaderType BINReader<T>::get_type() const {
    return ReaderType::BIN_READER;
}

template <class T>
std::unique_ptr<Reader> BINReader<T>::clone() const {
    return std::make_unique<BINReader<T>>(*this);
}

template <class T>
void BINReader<T>::accept(ReaderVisitor& visitor) const {
    visitor.visit(*this);
}

template <class T>
const BINFile<T>& BINReader<T>::get_file() const {
    return this->bin_file;
}