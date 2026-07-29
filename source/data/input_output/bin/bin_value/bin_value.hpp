#pragma once

#include <ostream>

template <class T>
struct BINValue {
public:
    T value = T{};

    void print(std::ostream& os) const;

    template <class U>
    friend std::ostream& operator << (std::ostream& os, const BINValue<U>& bin_value);
};

template <class T>
void BINValue<T>::print(std::ostream& os) const {
    os << this->value;
}

template <class T>
std::ostream& operator << (std::ostream& os, const BINValue<T>& bin_value) {
    bin_value.print(os);
    return os;
}
