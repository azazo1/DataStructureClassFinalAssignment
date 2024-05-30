//
// Created by azazo1 on 2024/5/29.
//

#ifndef LINKEDLIST_H
#define LINKEDLIST_H
#include <ostream>
#include <exception>

namespace mylinklist {
    class EmptyException : public std::exception {
        const char *what() const noexcept override {
            return "Emtpy LinkedList";
        }
    };

    template<class T>
    struct Node {
        T val;
        Node *next;

        Node();

        Node(const Node &other) = delete;

        explicit Node(const T &val, Node<T> *link = nullptr);

        ~Node();
    };

    enum ErrorCode {
        SUC, EMPTY, RANGE_ERROR, DUPLICATE
    };

    template<class T>
    class Iterator {
        const Node<T> *startNode;

    public:
        Iterator(const Node<T> *startNode);

        bool hasNext() const;

        T next();
    };

    template<class T>
    class LinkedList {
    public:
        LinkedList();

        LinkedList(const LinkedList &other);

        LinkedList<T> &operator=(const LinkedList<T> &other);

        bool full() const;

        int size() const;

        bool empty() const;

        void clear();

        void traverse(void (*fn)(T &));

        ErrorCode retrieve(int position, T &val) const;

        ErrorCode replace(int position, const T &val);

        ErrorCode remove(int position, T &disposed);

        ErrorCode insert(int position, const T &val);

        ErrorCode push_back(const T &val);

        T pop_head();

        /**
         * 在列表末尾添加元素, 如果元素已经在列表中出现则不添加.
         * @param val 要添加的元素
         * @return 错误代码, 成功添加为 SUC, 重复了则为 DUPLICATE.
         */
        ErrorCode push_back_unique(const T &val);

        bool contains(const T &target);

        ~LinkedList();

        Iterator<T> getIter() const;

    protected:
        int count = 0;
        Node<T> *head = nullptr;
        Node<T> *tail = nullptr;

        Node<T> *getNode(int position) const;
    };
} // mylinklist

#include "LinkedList.tpp"
#endif //LINKEDLIST_H
