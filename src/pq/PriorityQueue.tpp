#ifndef PRIORITYQUEUE_TPP
#define PRIORITYQUEUE_TPP


#include <cmath>

namespace pq {
    template<class T>
    size_t PriorityQueue<T>::getHeight() const {
        return static_cast<int>(log2(eleCnt));
    }

    template<class T>
    size_t PriorityQueue<T>::getSize() const {
        return eleCnt;
    }

    template<class T>
    PriorityQueue<T>::PriorityQueue(bool (*cmp)(T, T)): PriorityQueue(3, cmp) {
    }

    template<class T>
    PriorityQueue<T>::PriorityQueue(size_t initialCapacity, bool (*cmp)(T, T))
        : cmp(cmp),
          arr(new T[initialCapacity]),
          capacity(initialCapacity),
          eleCnt(0) {
    }

    template<class T>
    PriorityQueue<T>::PriorityQueue(const PriorityQueue &other)
        : cmp(nullptr),
          arr(new T[other.capacity]),
          capacity(other.capacity),
          eleCnt(other.eleCnt) {
        cmp = other.cmp;
        for (int i = 0; i < eleCnt; i++) {
            arr[i] = other.arr[i];
        }
    }

    template<class T>
    PriorityQueue<T> &PriorityQueue<T>::operator=(const PriorityQueue &other) {
        if (this == &other) {
            return *this;
        }
        cmp = other.cmp;
        capacity = other.capacity;
        eleCnt = other.eleCnt;
        delete[] arr; // 注意旧内存的回收
        arr = new T[capacity];
        for (int i = 0; i < eleCnt; i++) {
            arr[i] = other.arr[i];
        }
        return *this;
    }

    template<class T>
    PriorityQueue<T>::~PriorityQueue() {
        delete[] arr;
        eleCnt = 0;
        capacity = 0;
    }

    template<class T>
    void PriorityQueue<T>::reserve(size_t newCapacity) {
        if (capacity >= newCapacity) {
            return;
        }
        T *newArr = new T[newCapacity];
        for (int i = 0; i < eleCnt; i++) {
            newArr[i] = arr[i];
        }
        capacity = newCapacity;
        delete[] arr;
        arr = newArr;
    }

    template<class T>
    T PriorityQueue<T>::get() const {
        return arr[0];
    }

    template<class T>
    void PriorityQueue<T>::add(T ele) {
        if (eleCnt == capacity) {
            //        capacity *= 2;
            //        reserve(capacity);
            reserve(capacity * 2); // 前面那两行是错的, 不应该提前更改capacity大小, 否则会导致reserve内的误判
        }
        arr[eleCnt] = ele;
        swim(eleCnt++);
    }

    template<class T>
    void PriorityQueue<T>::swim(int index) {
        T moved = arr[index];
        int parent = (index - 1) / 2;
        while (parent != index && cmp(arr[parent], moved)) {
            arr[index] = arr[parent];
            index = parent;
            parent = (index - 1) / 2;
        }
        arr[index] = moved;
    }

    template<class T>
    void PriorityQueue<T>::sink(int index) {
        T moved = arr[index];
        int son = index * 2 + 1;
        while (son < eleCnt) {
            if (son < eleCnt - 1 && cmp(arr[son], arr[son + 1])) {
                son++;
            }
            if (cmp(moved, arr[son])) {
                arr[index] = arr[son];
                index = son;
                son = index * 2 + 1;
            } else {
                break;
            }
        }
        arr[index] = moved;
    }

    template<class T>
    T PriorityQueue<T>::pop() {
        if (eleCnt == 0) {
            throw EmptyPQException();
        }
        eleCnt--;
        T rst = arr[0];
        arr[0] = arr[eleCnt];
        sink(0);
        return rst;
    }
}

#endif
