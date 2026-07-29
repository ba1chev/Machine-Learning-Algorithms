#pragma once

#include <vector>
#include <ostream>
#include <stdexcept>

#include "source/data/input_output/bin/bin_record/bin_record.hpp"

template <class T>
struct BINFile {
public:
    std::vector<BINRecord<T>> file_data;
    size_t count_of_values = 0;

    void add_record(const BINRecord<T>& bin_record);
    void remove_record(size_t index);
    size_t get_count_of_records() const;
    size_t get_count_of_values() const;
    const BINRecord<T>& operator [] (size_t index) const;

    template <class U>
    friend std::ostream& operator << (std::ostream& os, const BINFile<U>& bin_file);
};

template <class T>
void BINFile<T>::add_record(const BINRecord<T>& bin_record) {
    if (this->file_data.empty()) {
        this->count_of_values = bin_record.get_count_of_values();
    } else if (bin_record.get_count_of_values() != this->count_of_values) {
        throw std::out_of_range("Input count of values is out of range");
    }

    this->file_data.push_back(bin_record);
}

template <class T>
void BINFile<T>::remove_record(size_t index) {
    if (index >= this->file_data.size()) {
        throw std::out_of_range("Index is out of range");
    }

    this->file_data.erase(this->file_data.begin() + index);
}

template <class T>
size_t BINFile<T>::get_count_of_records() const {
    return this->file_data.size();
}

template <class T>
size_t BINFile<T>::get_count_of_values() const {
    return this->count_of_values;
}

template <class T>
const BINRecord<T>& BINFile<T>::operator [] (size_t index) const {
    if (index >= this->file_data.size()) {
        throw std::out_of_range("Index is out of range");
    }

    return this->file_data[index];
}

template <class T>
std::ostream& operator << (std::ostream& os, const BINFile<T>& bin_file) {
    for (const BINRecord<T>& record : bin_file.file_data) {
        os << record << std::endl;
    }

    return os;
}
