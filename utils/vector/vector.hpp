#pragma once

#include <iostream>

template <class T>
class Vector {
private:
    T* data = nullptr;
    size_t size = 0;
    size_t capacity = 0;

    void free();
    void resize(size_t newCapacity);
    void copyFrom(const Vector& other);
    void moveTo(Vector&& other) noexcept;
    size_t calculateNeededCapacity(size_t size) const;
    
public:
    Vector();
    Vector(const T* data, size_t dataSize);
    Vector(size_t capacity);
    Vector(const Vector& other);
    Vector(Vector&& other) noexcept;
    Vector& operator = (const Vector& other);
    Vector& operator = (Vector&& other) noexcept;
    ~Vector();

    void pop_back();
    void push_back(const T& data);
    void push_back(T&& data);

    const T& operator [] (size_t index) const;
    T& operator [] (size_t index);

    size_t getSize() const;
    size_t getCapaciy() const;
    const T* getData() const;

    template <class U>
    friend std::ostream& operator << (std::ostream& os, const Vector<U>& vector);

    template <class U>
    friend std::istream& operator >> (std::istream& is, Vector<U>& vector);
};

template <class T>
Vector<T>::Vector() {
    this->capacity = 8;
    this->size = 0;
    this->data = new T[this->capacity] {};
}

template <class T>
Vector<T>::Vector(const T* data, size_t dataSize) {
    if (!data) {
        throw std::runtime_error("Nullptr detected");
    }

    this->size = dataSize;
    this->capacity = calculateNeededCapacity(this->size);
    this->data = new T[this->capacity] {};

    for (size_t i = 0; i < this->size; i++) {
        this->data[i] = data[i];
    }
}

template <class T>
Vector<T>::Vector(size_t capacity) {
    this->size = 0;
    this->capacity = capacity;
    this->data = new T[this->capacity] {};
}

template <class T>
Vector<T>::Vector(const Vector& other) {
    this->copyFrom(other);
}

template <class T>
Vector<T>::Vector(Vector&& other) noexcept {
    this->moveTo(std::move(other));
}

template <class T>
Vector<T>& Vector<T>::operator = (const Vector& other) {
    if (this != &other) {
        this->free();
        this->copyFrom(other);
    }

    return *this;
}

template <class T>
Vector<T>& Vector<T>::operator = (Vector&& other) noexcept {
    if (this != &other) {
        this->free();
        this->moveTo(std::move(other));
    }

    return *this;
}

template <class T>
Vector<T>::~Vector() {
    this->free();
}


template <class T>
void Vector<T>::pop_back() {
    if (this->size == 0) {
        throw std::runtime_error("The vector is already empty");
    }

    this->data[this->size - 1] = T{};
    this->size -= 1;
}

template <class T>
void Vector<T>::push_back(const T& data) {
    if (this->size == this->capacity) {
        this->resize(this->calculateNeededCapacity(this->size));
    }

    this->data[this->size] = data;
    this->size += 1;
}

template <class T>
void Vector<T>::push_back(T&& data) {
    if (this->size == this->capacity) {
        this->resize(this->calculateNeededCapacity(this->size));
    }

    this->data[this->size] = std::move(data);
    this->size += 1;
}

template <class T>
size_t Vector<T>::getSize() const {
    return this->size;
}

template <class T>
size_t Vector<T>::getCapaciy() const {
    return this->capacity;
}

template <class T>
const T* Vector<T>::getData() const {
    return this->data;
}

template <class T>
const T& Vector<T>::operator [] (size_t index) const {
    if (index >= this->size) {
        throw std::out_of_range("Index is out of range");
    }

    return this->data[index];
}

template <class T>
T& Vector<T>::operator [] (size_t index) {
    if (index >= this->size) {
        throw std::out_of_range("Index is out of range");
    }
    
    return this->data[index];
}

template <class T>
std::ostream& operator << (std::ostream& os, const Vector<T>& vector) {
    os << vector.getSize() << ", ";

    for (size_t i = 0; i < vector.getSize(); i++) {
        if (i != vector.getSize() - 1) {
            os << vector[i] << ", ";
        } else {
            os << vector[i];
        }
    }

    return os;
}

template <class T>
std::istream& operator >> (std::istream& is, Vector<T>& vector) {
    size_t size = 0;
    T currentElement {};

    is >> size;
    Vector<T> newVector(vector.calculateNeededCapacity(size));

    for (size_t i = 0; i < size; i++) {
        is >> currentElement;
        newVector.push_back(currentElement);
    }
    
    vector = newVector;
    return is;
}

template <class T>
void Vector<T>::free() {
    delete[] this->data;
    this->data = nullptr;
    this->size = 0;
    this->capacity = 0;
}

template <class T>
void Vector<T>::resize(size_t newCapacity) {
    if (newCapacity <= this->capacity) {
        throw std::runtime_error("New capacity must be greater than the old one");
    }

    this->capacity = newCapacity;
    T* newData = new T[this->capacity] {};

    for (size_t i = 0; i < this->size; i++) {
        newData[i] = this->data[i];
    }
    
    delete[] data;
    data = newData;
}


template <class T>
void Vector<T>::copyFrom(const Vector& other) {
    this->size = other.size;
    this->capacity = other.getCapaciy();
    T* newData = new T[this->capacity] {};

    for (size_t i = 0; i < this->size; i++) {
        newData[i] = other.data[i];
    }

    delete[] data;
    data = newData;
}

template <class T>
void Vector<T>::moveTo(Vector&& other) noexcept {
    this->data = other.data;
    this->capacity = other.capacity;
    this->size = other.size;

    other.data = nullptr;
    other.capacity = 0;
    other.size = 0;
}

template <class T>
size_t Vector<T>::calculateNeededCapacity(size_t size) const {
    size_t newCapacity = 1 << 0;

    while (newCapacity <= size) {
        newCapacity <<= 1;
    }

    return newCapacity;
}