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
    /**
     * Inserts new pair in hash table
     * @param pair
     * @return 0 - inserted
     * @return 1 - pair with that key has already existed in hash table
     */
    virtual int insert(K key, V value) = 0;

    /**
     * Removes pair with given key from hash table
     * @param key
     * @return 0 - removed
     * @return 1 - not found
     */
    virtual int remove(int key) = 0;
};

#endif //HASH_TABLES_IHASHTABLE_H
