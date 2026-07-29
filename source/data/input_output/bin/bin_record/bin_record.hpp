#pragma once

#include <vector>
#include <ostream>
#include <stdexcept>

#include "source/data/input_output/bin/bin_value/bin_value.hpp"

template <class T>
struct BINRecord {
public:
    std::vector<BINValue<T>> record_data;

    void add_value(const T& value);
    void remove_value(size_t index);
    size_t get_count_of_values() const;

    template <class U>
    friend std::ostream& operator << (std::ostream& os, const BINRecord<U>& bin_record);
};

template <class T>
void BINRecord<T>::add_value(const T& value) {
    this->record_data.push_back(BINValue<T>{ value });
}

template <class T>
void BINRecord<T>::remove_value(size_t index) {
    if (index >= this->record_data.size()) {
        throw std::out_of_range("Index is out of range");
    }

    this->record_data.erase(this->record_data.begin() + index);
}

template <class T>
size_t BINRecord<T>::get_count_of_values() const {
    return this->record_data.size();
}

template <class T>
std::ostream& operator << (std::ostream& os, const BINRecord<T>& bin_record) {
    size_t count_of_values = bin_record.record_data.size();
    
    for (size_t i = 0; i < count_of_values; i++) {
        if (i > 0) {
            os << ",";
        }
        bin_record.record_data[i].print(os);
    }

    return os;
}
