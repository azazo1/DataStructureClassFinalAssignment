//
// Created by azazo1 on 2024/5/29.
//

#include "HashSet.h"

namespace myhashset {
    HashSet::HashSet(const int hashSize)
        : hashSize(hashSize),
          count(0),
          // table(new mylinklist::LinkedList<int>[hashSize]) {
          table(new vec::Vec<int>[hashSize]) {
    }

    HashSet::~HashSet() {
        delete[] table;
    }

    vec::Vec<int> HashSet::flatten() const {
        vec::Vec<int> rst(count);
        for (int i = 0; i < hashSize; i++) {
            // mylinklist::LinkedList<int> &lst = table[i];
            // const int size = lst.size();
            vec::Vec<int> &vec = table[i];
            const int size = vec.getLength();
            for (int j = 0; j < size; j++) {
                // int t;
                // lst.retrieve(j, t);
                int t = vec[j];
                rst.push_back(t);
            }
        }
        return rst;
    }

    int HashSet::hash(const int val) const {
        return val % hashSize;
    }

    bool HashSet::contains(const int target) const {
        const int idx = hash(target);
        // mylinklist::LinkedList<int> &lst = table[idx];
        // return lst.contains(target);
        return table[idx].contains(target);
    }

    void HashSet::put(const int val) {
        const int idx = hash(val);
        // mylinklist::LinkedList<int> &lst = table[idx];
        // lst.insert(0, val);
        table[idx].push_back(val);
        count++;
    }
} // myhashset
