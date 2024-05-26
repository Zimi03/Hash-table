//
// Created by grzegorz on 24.05.24.
//
#include "HashTableArray.h"


template <typename K, typename V>
HashTableArray<K, V>::HashTableArray(){
    load_factor = 0;
    array = new DynamicArray<Pair<K, V>>*[capacity] {nullptr};
}


template <typename K, typename V>
HashTableArray<K, V>::HashTableArray(HashTableArray<K, V> *to_copy) {
    capacity = to_copy->capacity;
    size = to_copy->size;
    load_factor = to_copy->load_factor;
    load_factor_max = to_copy->load_factor_max;

    array = new DynamicArray<Pair<K, V>>*[capacity] {nullptr};
    for(int i =0 ; i < capacity; i++){
//        array[i] = to_copy->array[i] ? new DynamicArray<Pair<K, V>>(*to_copy->array[i]) : nullptr;
        if (to_copy->array[i] != nullptr) {
            array[i] = new DynamicArray<Pair<K, V>>(to_copy->array[i]);
        }
    }

}


template <typename K, typename V>
HashTableArray<K, V>::~HashTableArray() {
//    for (int i = 0; i < capacity; i++) {
//        if (array[i] != nullptr) delete array[i];
//    }
    delete[] array;
}


template <typename K, typename V>
int HashTableArray<K, V>::hashfunction1(K key){
    int x = key % capacity;
    return x;
}

template <typename K, typename V>
int HashTableArray<K, V>::hashfunction2(K key){
    int x = (key/capacity) % capacity;
    return x;
}

template <typename K, typename V>
int HashTableArray<K, V>::rehash() {
    int old_capacity = capacity;
    capacity*=2;
    DynamicArray<Pair<K, V>>** tmp = array;
    array = new DynamicArray<Pair<K, V>>*[capacity] {nullptr};
    size = 0;

    for(int i = 0; i < old_capacity; i ++) {
        if(tmp[i]== nullptr) continue;
        int array_size = tmp[i]->getSize();
        for(int j = 0; j < array_size; j++) {
            std::optional<Pair<K, V>> to_move = tmp[i]->removeBack();
            if (to_move != std::nullopt) insert(to_move.value().key, to_move.value().value);
        }
        delete tmp[i];
    }

    delete [] tmp;
    return 0;
}

template <typename K, typename V>
int HashTableArray<K, V>::grow() {
    size++;
    load_factor = float(size) / capacity;
    if(load_factor > load_factor_max)
        return rehash();
    return 2;
}

template <typename K, typename V>
//int HashTableArray<K, V>::insert(Pair<K, V> pair) {
int HashTableArray<K, V>::insert(K key, V value) {

    int indeks1 = hashfunction1(key);
    int indeks2 = hashfunction2(key);
    Pair<K, V> pair(key, value);

    // sprawdzenie czy już jest element o takim kluczu
    if(array[indeks1] != nullptr && array[indeks1]->find(pair) != -1) return 1;
    if(array[indeks2] != nullptr && array[indeks2]->find(pair) != -1) return 1;

    if(array[indeks1] == nullptr){
        array[indeks1] = new DynamicArray<Pair<K, V>>;
        array[indeks1]->insertBack(pair);
    } else if(array[indeks2] == nullptr){
        array[indeks2] = new DynamicArray<Pair<K, V>>;
        array[indeks2]->insertBack(pair);
    } else if( array[indeks1]->getSize() <= array[indeks2]->getSize()){
        array[indeks1]->insertBack(pair);
    } else {
        array[indeks2]->insertBack(pair);
    }
    grow();
    return 0;
}



template <typename K, typename V>
int HashTableArray<K, V>::remove(int key) {
    int indeks1 = hashfunction1(key);
    if(array[indeks1] != nullptr ){
        Pair<K, V> pair (key,0);
        int indeks = array[indeks1]->find(pair);
        if( indeks != -1) {
            array[indeks1]->remove(indeks);
            if(array[indeks1]->isEmpty()) {
                delete array[indeks1];
                array[indeks1] = nullptr;
            }
            return 0;
        }
    }

    int indeks2 = hashfunction2(key);
    if(array[indeks2] != nullptr ){
        Pair<K, V> pair (key,0);
        int indeks = array[indeks2]->find(pair);
        if( indeks != -1) {
            array[indeks2]->remove(indeks);
            if(array[indeks2]->isEmpty()) {
                delete array[indeks2];
                array[indeks2] = nullptr;
            }
            return 0;
        }
    }

    return 1; // Element nieznaleziony
}


template <typename K, typename V>
void HashTableArray<K, V>::display() {
    for(int i = 0; i < capacity; i++) {
        if (array[i] == nullptr) {
            std::cout << "Index: " << i << " | " << "empty" << std::endl;
        } else {
            int array_size = array[i]->getSize();
            std::cout << "Index: " << i;
            for (int j = 0; j < array_size; j++) {
                std::optional<Pair<K, V>> para = array[i]->get(j);
                if (para != std::nullopt) {
                    std::cout << " | " << para.value();
                }
            }
            std::cout << std::endl;
        }
    }
}


template class HashTableArray <int, int>;