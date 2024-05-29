//
// Created by azazo1 on 2024/5/29.
//

#ifndef LINKED_LIST_TPP
#define LINKED_LIST_TPP

#include <ostream>

#include "LinkedList.h"

namespace mylinklist {
    template<class T>
    bool LinkedList<T>::full() const {
        auto *cur = new Node<T>();
        return cur == nullptr;
    }

    template<class T>
    LinkedList<T>::LinkedList(): count(0), head(nullptr) {
    }

    template<class T>
    LinkedList<T>::LinkedList(const LinkedList &other): count(0), head(nullptr) {
        *this = other;
    }

    template<class T>
    LinkedList<T> &LinkedList<T>::operator=(const LinkedList<T> &other) {
        if (this == &other) {
            return *this;
        }
        clear();
        Node<T> *oNode = other.head;
        Node<T> *sEnd = nullptr;
        while (oNode != nullptr) {
            if (head == nullptr) {
                head = new Node<T>(oNode->val);
                sEnd = head;
            } else {
                sEnd->next = new Node<T>(oNode->val);
                sEnd = sEnd->next;
            }
            oNode = oNode->next;
        }
        return *this;
    }

    template<class T>
    int LinkedList<T>::size() const {
        return count;
    }

    template<class T>
    bool LinkedList<T>::empty() const {
        return count == 0;
    }

    template<class T>
    void LinkedList<T>::clear() {
        delete head;
        head = nullptr;
        count = 0;
    }

    template<class T>
    void LinkedList<T>::traverse(void (*fn)(T &)) {
        Node<T> *cur = head;
        while (cur != nullptr) {
            fn(cur->val);
            cur = cur->next;
        }
    }

    template<class T>
    ErrorCode LinkedList<T>::retrieve(int position, T &val) const {
        if (count == 0) {
            return EMPTY;
        }
        if (position < 0 || position >= count) {
            return RANGE_ERROR;
        }
        Node<T> *cur = getNode(position);
        val = cur->val;
        return SUC;
    }

    template<class T>
    ErrorCode LinkedList<T>::replace(int position, const T &val) {
        if (count == 0) {
            return EMPTY;
        }
        if (position < 0 || position >= count) {
            return RANGE_ERROR;
        }
        Node<T> *cur = getNode(position);
        cur->val = val;
        return SUC;
    }

    template<class T>
    ErrorCode LinkedList<T>::remove(int position, T &disposed) {
        if (count == 0) {
            return EMPTY;
        }
        if (position < 0 || position >= count) {
            return RANGE_ERROR;
        }
        if (position == 0) {
            Node<T> *n = head->next;
            head->next = nullptr; // in case free the later node (see ~Node)
            disposed = head->val;
            delete head;
            head = n;
        } else {
            Node<T> *prev = getNode(position - 1);
            Node<T> *cur = prev->next;
            disposed = cur->val;
            prev->next = cur->next;
            cur->next = nullptr;
            delete cur;
        }
        count--;
        return SUC;
    }

    template<class T>
    ErrorCode LinkedList<T>::insert(const int position, const T &val) {
        if (position < 0 || position > count) {
            return RANGE_ERROR;
        }
        if (position == 0) {
            Node<T> *h = head;
            head = new Node<T>(val);
            head->next = h;
        } else {
            Node<T> *prev = getNode(position - 1);
            auto *cur = new Node<T>(val);
            cur->next = prev->next;
            prev->next = cur;
        }
        count++;
        return SUC;
    }

    template<class T>
    Node<T> *LinkedList<T>::getNode(int position) const {
        Node<T> *cur = head;
        for (int i = 0; i < position; i++) {
            cur = cur->next;
        }
        return cur;
    }

    template<class T>
    std::ostream &operator<<(std::ostream &out, const LinkedList<T> &list) {
        out << "[";
        Node<T> *cur = list.head;
        while (cur != nullptr) {
            out << cur->val;
            cur = cur->next;
            if (cur == nullptr) {
                out << "]";
            } else {
                out << ", ";
            }
        }
        return out;
    }

    template<class T>
    LinkedList<T>::~LinkedList() {
        count = 0;
        delete head;
        head = nullptr;
    }

    template<class T>
    bool LinkedList<T>::contains(const T &target) {
        Node<T> *cur = head;
        while (cur != nullptr) {
            if (cur->val == target) {
                return true;
            }
            cur = cur->next;
        }
        return false;
    }

    template<class T>
    ErrorCode LinkedList<T>::push_back(const T &val) {
        return insert(count, val);
    }

    template<class T>
    ErrorCode LinkedList<T>::push_back_unique(const T &val) {
        Node<T> *cur = head;
        if (cur == nullptr) {
            head = new Node<T>(val);
        } else {
            while (cur->next != nullptr) {
                if (cur->val == val) {
                    return DUPLICATE;
                }
                cur = cur->next;
            }
            cur->next = new Node<T>(val);
        }
        count++;
        return SUC;
    }
} // mylinklist

#endif
