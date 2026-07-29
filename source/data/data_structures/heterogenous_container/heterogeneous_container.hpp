#pragma once

#include <utility>
#include <stdexcept>

template <class T>
class HeterogeneousContainer {
private:
	T** data = nullptr;
	size_t size = 0;
	size_t capacity = 8;

	void resize(size_t newCapacity);
	void copy_from(const HeterogeneousContainer& other);
    void move_to(HeterogeneousContainer&& other) noexcept;
    void free();

public:
	HeterogeneousContainer();
	HeterogeneousContainer(const HeterogeneousContainer<T>& other);
	HeterogeneousContainer(HeterogeneousContainer&& other) noexcept;
	HeterogeneousContainer& operator = (const HeterogeneousContainer& other);
	HeterogeneousContainer& operator = (HeterogeneousContainer&& other) noexcept;
	~HeterogeneousContainer();

	void add_element(const T& element);
	void add_element(const T* element);
    void add_element(T&& element);
	void remove_element(size_t index);

	size_t get_size() const;
    size_t get_capacity() const;

	T* operator [] (size_t index);
	const T* operator [] (size_t index) const;
};

template <class T>
HeterogeneousContainer<T>::HeterogeneousContainer() {
    this->capacity = 8;
    this->size = 0;
    this->data = new T*[this->capacity] {nullptr};
}

template <class T>
HeterogeneousContainer<T>::HeterogeneousContainer(const HeterogeneousContainer<T>& other) {
    this->copy_from(other);
}

template <class T>
HeterogeneousContainer<T>::HeterogeneousContainer(HeterogeneousContainer&& other) noexcept {
    this->move_to(std::move(other));
}

template <class T>
HeterogeneousContainer<T>& HeterogeneousContainer<T>::operator = (const HeterogeneousContainer& other) {
    if (this != &other) {
        this->free();
        this->copy_from(other);
    }

    return *this;
}

template <class T>
HeterogeneousContainer<T>& HeterogeneousContainer<T>::operator = (HeterogeneousContainer&& other) noexcept {
    if (this != &other) {
        this->free();
        this->move_to(std::move(other));
    }

    return *this;
}

template <class T>
HeterogeneousContainer<T>::~HeterogeneousContainer() {
    this->free();
}

template <class T>
void HeterogeneousContainer<T>::add_element(const T& element) {
    if (this->size == this->capacity) {
        this->resize(this->size == 0 ? 8 : this->size * 2);
    }

    // Preserve dynamic type by cloning when T is polymorphic.
    this->data[this->size] = element.clone().release();
    this->size += 1;
}

template <class T>
void HeterogeneousContainer<T>::add_element(T&& element) {
    if (this->size == this->capacity) {
        this->resize(this->size == 0 ? 8 : this->size * 2);
    }

    this->data[this->size] = element.clone().release();
    this->size += 1;
}

template <class T>
void HeterogeneousContainer<T>::add_element(const T* element) {
    if (this->size == this->capacity) {
        this->resize(this->size == 0 ? 8 : this->size * 2);
    }

    this->data[this->size] = element->clone().release();
    this->size += 1;
}

template <class T>
void HeterogeneousContainer<T>::remove_element(size_t index) {
    if (index >= this->size) throw std::out_of_range("Index is out of range");
    delete this->data[index];

    for (size_t i = index; i < this->size - 1; i++) {
        this->data[i] = this->data[i + 1];
    }
    
    data[size - 1] = nullptr;
    this->size -= 1;
}

template <class T>
size_t HeterogeneousContainer<T>::get_size() const {
    return this->size;
}

template <class T>
size_t HeterogeneousContainer<T>::get_capacity() const {
    return this->capacity;
}

template <class T>
void HeterogeneousContainer<T>::resize(size_t new_capacity) {
    if (new_capacity <= this->capacity) {
        throw std::logic_error("New capacity must be greater than old capacity");
    }

    T** newData = new T*[new_capacity] {nullptr};
    for (size_t i = 0; i < this->size; i++) {
        newData[i] = this->data[i];
        this->data[i] = nullptr;
    }
    
    delete[] this->data;
    this->data = newData;
    this->capacity = new_capacity;
}

template <class T>
void HeterogeneousContainer<T>::copy_from(const HeterogeneousContainer& other) {
    this->data = new T*[other.capacity] {nullptr};
    this->size = other.size;
    this->capacity = other.capacity;

    for (size_t i = 0; i < this->size; i++) {
        this->data[i] = other.data[i]->clone().release();
    }
}

template <class T>
void HeterogeneousContainer<T>::move_to(HeterogeneousContainer&& other) noexcept {
    this->data = other.data;
    this->size = other.size;
    this->capacity = other.capacity;

    other.data = nullptr;
    other.size= 0;
    other.capacity = 0;
}

template <class T>
void HeterogeneousContainer<T>::free() {
    for (size_t i = 0; i < this->size; i++) {
        delete this->data[i];
        this->data[i] = nullptr;
    }

    delete[] this->data;
    this->data = nullptr;
    this->size = 0;
    this->capacity = 0;
}

template <class T>
T* HeterogeneousContainer<T>::operator [] (size_t index) {
    if (index >= this->size) throw std::out_of_range("Index is out of range");
    return this->data[index];
}

template <class T>
const T* HeterogeneousContainer<T>::operator [] (size_t index) const {
    if (index >= this->size) throw std::out_of_range("Index is out of range");
    return this->data[index];
}