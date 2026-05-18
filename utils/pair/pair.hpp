#pragma once

template <class T1, class T2>
class Pair {
private:
    T1 first;
    T2 second;

public:
    Pair(const T1& first, const T2& second);
    bool operator <= (const Pair& other) const;
    bool operator >= (const Pair& other) const;
    bool operator == (const Pair& other) const;

    const T1& getFirst() const;
    const T2& getSecond() const;
};

template <class T1, class T2>
bool operator < (const Pair<T1, T2>& left, const Pair<T1, T2>& right) {
    return !(left >= right);
}

template <class T1, class T2>
bool operator > (const Pair<T1, T2>& left, const Pair<T1, T2>& right) {
    return !(left <= right);
}

template <class T1, class T2>
bool operator != (const Pair<T1, T2>& left, const Pair<T1, T2>& right) {
    return !(left == right);
}

template <class T1, class T2>
Pair<T1, T2>::Pair(const T1& first, const T2& second) : first(first), second(second) {}

template <class T1, class T2>
bool Pair<T1, T2>::operator <= (const Pair& other) const {
    if (this->first == other.getFirst()) {
        return this->second <= other.getSecond();
    }

    return this->first < other.getFirst();
}

template <class T1, class T2>
bool Pair<T1, T2>::operator >= (const Pair& other) const {
    if (this->first == other.getFirst()) {
        return this->second >= other.getSecond();
    }

    return this->first > other.getFirst();
}

template <class T1, class T2>
bool Pair<T1, T2>::operator == (const Pair& other) const {
    if (this->first == other.getFirst()) {
        return this->second == other.getSecond();
    }

    return false;
}
    
template <class T1, class T2>
const T1& Pair<T1, T2>::getFirst() const {
    return this->first;
}
    
template <class T1, class T2>
const T2& Pair<T1, T2>::getSecond() const {
    return this->second;
}