//
// Created by azazo1 on 2024/5/30.
//

#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H

namespace pq {
    /**
     * 采用线性存储
     */
    template<class T>
    class PriorityQueue {
    public:
        PriorityQueue(bool (*cmp)(T, T));

        PriorityQueue(size_t initialCapacity, bool (*cmp)(T, T));

        PriorityQueue(const PriorityQueue &other);

        PriorityQueue<T> &operator=(const PriorityQueue &other);

        ~PriorityQueue();

        /**
         * 讲一个元素添加到优先级队列.
         */
        void add(T ele);

        /**
         * 返回优先级队列顶部的元素.
         */
        T get() const;

        /**
         * 增大内部数组容量, 此函数不会缩减容量.
         * @param newCapacity 新的容量大小
         */
        void reserve(size_t newCapacity);

        /**
         * 弹出优先级队列的顶部元素.
         */
        T pop();

        size_t getHeight() const;

        size_t getSize() const;

    private:
        /**
         * 将位于 index 中的元素下降到它应该到的地方.
         */
        void swim(int index);

        /**
         * 将位于 index 中的元素提升到它应该到的地方.
         */
        void sink(int index);

        /**
         * 用于两个元素的比较, 如果第一个参数小于第二个参数, 应该返回 true.
         * @note 此方法中最大的元素将会在堆顶.
         */
        bool (*cmp)(T, T);

        T *arr;
        size_t capacity;
        size_t eleCnt;
    };
}

#include "PriorityQueue.tpp"

#endif //PRIORITYQUEUE_H
