#pragma once

#include <string>
#include <fstream>
#include <stdexcept>

#include "source/data/input_output/writers/writer.hpp"
#include "source/data/input_output/bin/bin_file/bin_file.hpp"

template <class T>
class BINWriter: public Writer<BINFile<T>> {
public:
    BINWriter(const std::string& filename);
    
    void write(const BINFile<T>& data) const override;
    std::unique_ptr<Writer<BINFile<T>>> clone() const override;
    WriterType get_type() const override;
};

template <class T>
BINWriter<T>::BINWriter(const std::string& filename): Writer<BINFile<T>>(filename) {}

template <class T>
void BINWriter<T>::write(const BINFile<T>& data) const {
    std::ofstream os(this->filename, std::ios::binary);
    if (!os) {
        throw std::runtime_error("Could not open output file for writing");
    }

    size_t count_of_records = data.get_count_of_records();
    size_t values_per_record = data.get_count_of_values();
    os.write(reinterpret_cast<const char*>(&count_of_records), sizeof(count_of_records));
    os.write(reinterpret_cast<const char*>(&values_per_record), sizeof(values_per_record));

    for (size_t i = 0; i < count_of_records; i++) {
        const BINRecord<T>& record = data[i];
        
        for (size_t j = 0; j < values_per_record; j++) {
            T value = record.record_data[j].value;
            os.write(reinterpret_cast<const char*>(&value), sizeof(value));
        }
    }
}

template <class T>
WriterType BINWriter<T>::get_type() const {
    return WriterType::BIN_WRITER;
}

template <class T>
std::unique_ptr<Writer<BINFile<T>>> BINWriter<T>::clone() const {
    return std::make_unique<BINWriter<T>>(*this);
}
