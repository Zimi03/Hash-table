//
// Created by grzegorz on 23.05.24.
//

#ifndef HASH_TABLES_IHASHTABLE_H
#define HASH_TABLES_IHASHTABLE_H
#include "../Structures/Pair.hpp"


template <typename K, typename V>
class IHashTable {
public:
    virtual ~IHashTable() {};
    virtual int insert(Pair<K, V> pair) = 0;
    virtual int remove(int key) = 0;
};

#endif //HASH_TABLES_IHASHTABLE_H
