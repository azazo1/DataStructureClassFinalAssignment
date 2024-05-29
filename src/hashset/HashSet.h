//
// Created by azazo1 on 2024/5/29.
//

#ifndef HASHSET_H
#define HASHSET_H
#include "../linkedlist/LinkedList.h"
#include "../vec/Vec.h"

namespace myhashset {
    class HashSet {
        int hashSize;
        int count;
        // mylinklist::LinkedList<int> *table;
        vec::Vec<int> *table;

        int hash(int val) const;

    public:
        explicit HashSet(int hashSize = 20000);

        ~HashSet();

        /**
         * 把所有结果扁平化成一个序列
         */
        vec::Vec<int> flatten() const;

        bool contains(int target) const;

        void put(int val);
    };
} // myhashset

#endif //HASHSET_H
