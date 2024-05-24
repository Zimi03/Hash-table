//
// Created by grzegorz on 24.05.24.
//

#ifndef HASH_TABLES_HASHTABLEARRAY_H
#define HASH_TABLES_HASHTABLEARRAY_H
#include "IHashTable.h"
#include "../Structures/DynamicArray.hpp"


template <typename K, typename  V>
class HashTableArray: IHashTable<K, V> {
    DynamicArray<Pair<K, V>>** array;
    int capacity = 10;
    int size = 0;
    float load_factor;
    float load_factor_max = 2;


    int hashfunction1(K key);
    int hashfunction2(K key);
    int rehash();

    int grow();

public:
    HashTableArray();
    explicit HashTableArray(HashTableArray<K, V> &to_copy);
    ~HashTableArray() override;
//    int insert(Pair<K, V> pair) override;
    int insert(K key, V value) override;
    int remove(int key) override;
    void display();

};


#endif //HASH_TABLES_HASHTABLEARRAY_H
