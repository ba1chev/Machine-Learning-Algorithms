#pragma once

#include <memory>
#include <string>
#include <stdexcept>
#include <type_traits>

#include "source/data/input_output/writers/writer.hpp"
#include "source/data/input_output/csv/csv_table/csv_table.h"
#include "source/data/input_output/writers/csv_writer/csv_writer.h"
#include "source/data/input_output/writers/writer_factory/utils.hpp"
#include "source/data/input_output/writers/bin_writer/bin_writer.hpp"
#include "source/data/input_output/writers/writer_factory/abstract_writer_factory.hpp"

template <class T>
class WriterFactory: public AbstractWriterFactory<T> {
public:
    std::unique_ptr<Writer<T>> create(WriterType writer_type, const std::string& filename) const override;
};

template <class T>
std::unique_ptr<Writer<T>> WriterFactory<T>::create(WriterType writer_type, const std::string& filename) const {
    switch (writer_type) {
        case WriterType::CSV_WRITER: {
            if constexpr (std::is_same_v<T, CSVTable>) {
                return std::make_unique<CSVWriter>(filename);
            } else {
                throw std::runtime_error("CSV writer requires a CSVTable data type");
            }
        }
        case WriterType::BIN_WRITER: {
            if constexpr (WRITER_FACTORY_DETAIL::is_bin_file<T>::value) {
                return std::make_unique<BINWriter<typename WRITER_FACTORY_DETAIL::bin_value_type<T>::type>>(filename);
            } else {
                throw std::runtime_error("Binary writer requires a BINFile data type");
            }
        }
        default: throw std::runtime_error("Not supported writer type");
    }
}
