//
// Created by azazo1 on 2024/5/29.
//
#ifndef END_HOMEWORK_VEC_TPP
#define END_HOMEWORK_VEC_TPP

#ifdef DEBUG_VEC_SET_LENGTH
#include <iostream>
#endif

namespace vec {
    template<class T>
    Vec<T>::Vec(const int initialSize)
        : arr(new T[initialSize]),
          capacity(initialSize),
          length(0) {
    }

    template<class T>
    T &Vec<T>::operator[](int idx) {
        return arr[idx];
    }

    template<class T>
    const T &Vec<T>::operator[](int idx) const {
        return arr[idx];
    }

    template<class T>
    void Vec<T>::push_back(const T &val) {
        if (length == capacity) {
            reallocate(capacity * 2);
        }
        arr[length++] = val;
    }

    template<class T>
    void Vec<T>::reallocate(const int newCapacity) {
        if (newCapacity <= capacity) {
            return;
        }
        capacity = newCapacity;
        T *newArr = new T[newCapacity];
        for (int i = 0; i < length; i++) {
            newArr[i] = arr[i];
        }
        delete[] arr;
        arr = newArr;
    }

    template<class T>
    int Vec<T>::getCapacity() const {
        return capacity;
    }

    template<class T>
    int Vec<T>::getLength() const {
        return length;
    }

    template<class T>
    Vec<T>::~Vec() {
        delete[] arr;
        length = 0;
        capacity = 0;
    }

    template<class T>
    bool Vec<T>::contains(const T &target) const {
        for (int i = 0; i < length; i++) {
            if (target == arr[i]) {
                return true;
            }
        }
        return false;
    }

    template<class T>
    Vec<T>::Vec(const Vec &other): Vec(other.capacity) {
        length = other.length;
        for (int i = 0; i < length; i++) {
            arr[i] = other.arr[i];
        }
    }

    template<class T>
    Vec<T> &Vec<T>::operator=(const Vec &other) {
        if (this == &other) {
            return *this;
        }
        reallocate(other.capacity);
        length = other.length;
        for (int i = 0; i < other.length; i++) {
            arr[i] = other.arr[i];
        }
        return *this;
    }

    template<class T>
    T *Vec<T>::begin() {
        return arr;
    }

    template<class T>
    T *Vec<T>::end() {
        return arr + length;
    }

    template<class T>
    void Vec<T>::setLength(int length) const {
#ifdef DEBUG_VEC_SET_LENGTH
        if (length > capacity) {
            std::cerr << "invalid length" << std::endl;
            exit(1001);
        }
#endif
        this->length = length;
    }
}

#endif
