//
// Created by grzegorz on 23.05.24.
//

#ifndef HASH_TABLES_CUCKOOHASHTABLE_H
#define HASH_TABLES_CUCKOOHASHTABLE_H
#include "../Structures/DynamicArray.hpp"
#include "IHashTable.h"

template <typename K, typename  V>
class CuckooHashTable: IHashTable<K, V> {
    Pair<K, V>* tab1;
    Pair<K, V>* tab2;
    int capacity;
    int size;
    int max_steps = 10;
    float load_factor;
    float load_factor_max = 0.5;


    int hashfunction1(K key);
    int hashfunction2(K key);
    int rehash();

    int grow();
    int shrink();
    int swap(Pair<K, V> &p1, Pair<K, V> &p2);
    int swap(int p1, int p2);


public:
    CuckooHashTable();
    ~CuckooHashTable() override;
    int insert(Pair<K, V> pair) override;
    int remove(int key) override;
    int display();

};




#endif //HASH_TABLES_CUCKOOHASHTABLE_H
