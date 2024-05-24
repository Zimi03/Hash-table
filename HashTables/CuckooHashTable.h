//
// Created by grzegorz on 23.05.24.
//

#ifndef HASH_TABLES_CUCKOOHASHTABLE_H
#define HASH_TABLES_CUCKOOHASHTABLE_H
#include "IHashTable.h"

template <typename K, typename  V>
class CuckooHashTable: IHashTable<K, V> {
    Pair<K, V>** tab1;
    Pair<K, V>** tab2;
    int capacity = 10;
    int size = 0;
    int max_steps = 20;
    float load_factor;
    float load_factor_max = 0.5;


    int hashfunction1(K key);
    int hashfunction2(K key);
    int rehash();

    int grow();

public:
    explicit CuckooHashTable(int max_steps_);
    CuckooHashTable();
    ~CuckooHashTable() override;
//    int insert(Pair<K, V> pair) override;
    int insert(K key, V value) override;
    int remove(int key) override;
    void display();

};




#endif //HASH_TABLES_CUCKOOHASHTABLE_H
