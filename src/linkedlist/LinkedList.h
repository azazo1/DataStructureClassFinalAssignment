//
// Created by azazo1 on 2024/5/29.
//

#ifndef LINKEDLIST_H
#define LINKEDLIST_H
#include <ostream>

namespace mylinklist {
    template<class T>
    struct Node {
        T val;
        Node *next;

        Node();

        Node(const Node &other) = delete;

        explicit Node(const T &val, Node<T> *link = nullptr);

        ~Node();
    };

    template<class T>
    Node<T>::Node(): val(), next(nullptr) {
    }

    template<class T>
    Node<T>::Node(const T &val, Node<T> *link): val(val), next(link) {
    }

    template<class T>
    Node<T>::~Node() {
        // 实际上这并不是一个很好的设计, 使其在拷贝 Node 时出现异常.
        delete next;
    }

    enum ErrorCode {
        SUC, EMPTY, RANGE_ERROR, DUPLICATE
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

        /**
         * 在列表末尾添加元素, 如果元素已经在列表中出现则不添加.
         * @param val 要添加的元素
         * @return 错误代码, 成功添加为 SUC, 重复了则为 DUPLICATE.
         */
        ErrorCode push_back_unique(const T &val);

        bool contains(const T &target);

        /**
         * Incorrect version:
         * friend ostream&operator<<(ostream &out, const LinkedList<T> &list);
         * when using "friend" keyword, the function after it has its own
         * generics, thus "template <class U>" is essential !!!
         */
        template<class U>
        friend std::ostream &operator<<(std::ostream &out, const LinkedList<U> &list);

        ~LinkedList();

    protected:
        int count;
        Node<T> *head;

        Node<T> *getNode(int position) const;
    };
} // mylinklist

#include "LinkedList.tpp"
#endif //LINKEDLIST_H
