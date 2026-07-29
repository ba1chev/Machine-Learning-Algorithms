#pragma once

#include <stdexcept>

#include "source/learning_models/scaling/scaler_type.h"
#include "source/data/data_structures/matrix/matrix.hpp"

template <class T>
class Scaler {
protected:
    Matrix<T> parameters;
    bool fitted = false;

public:
    virtual void fit(const Matrix<T>& data) = 0;
    virtual Matrix<T> transform(const Matrix<T>& data) const = 0;

    void set_parameters(const Matrix<T>& parameters);
    const Matrix<T>& get_parameters() const;
    bool is_fitted() const;

    virtual ScalerType get_type() const = 0;
    virtual ~Scaler() = default;
};

template <class T>
void Scaler<T>::set_parameters(const Matrix<T>& parameters) {
    this->parameters = parameters;
    this->fitted = true;
}

template <class T>
const Matrix<T>& Scaler<T>::get_parameters() const {
    if (!this->fitted) {
        throw std::runtime_error("Scaler has no parameters (not fitted)");
    }

    return this->parameters;
}

template <class T>
bool Scaler<T>::is_fitted() const {
    return this->fitted;
}
