//
// Created by grzegorz on 23.05.24.
//

#include "CuckooHashTable.h"


template <typename K, typename V>
CuckooHashTable<K, V>::CuckooHashTable() {
    tab1 = DynamicArray<Pair<K, V>*>(10);
    tab2 = DynamicArray<Pair<K, V>*>(10);
    capacity = tab1.getCapacity();
    size = tab1.getSize()+tab2.getSize();
    for(int i = 0; i < capacity; i++){
        tab1.insert(i, nullptr);
        tab2.insert(i, nullptr);
    }
}

template <typename K, typename V>
CuckooHashTable<K, V>::~CuckooHashTable() {
//    std::optional<Pair<K, V>*> to_remove;
//    for(int i = 0; i < capacity; i++){
//        to_remove = tab1.get(i);
//        if(to_remove != std::nullopt) {
//            if(to_remove.value() != nullptr)
//                delete to_remove.value();
//        }
//        to_remove = tab2.get(i);
//        if(to_remove != std::nullopt) {
//            if(to_remove.value() != nullptr)
//                delete to_remove.value();
//        }
//    }
}


template <typename K, typename V>
int CuckooHashTable<K, V>::hashfunction1(K key){
    return key % capacity;
}

template <typename K, typename V>
int CuckooHashTable<K, V>::hashfunction2(K key){
    return (key/capacity) % capacity;
}

template <typename K, typename V>
int CuckooHashTable<K, V>::rehash() {
    int old_capacity = tab1.getCapacity();
    capacity*=2;
    DynamicArray<Pair<K, V>*> tmp1(capacity);
    DynamicArray<Pair<K, V>*> tmp2(capacity);
    std::optional<Pair<K, V>*> to_move;

    for(int i = 0; i < old_capacity; i ++){
        to_move = tab1.removeBack();
        if(to_move == std::nullopt) return 1;
        else if( to_move.value() != nullptr){
            insert(*to_move.value());
        }
    }
    for(int i = 0; i < old_capacity; i ++){
        to_move = tab2.removeBack();
        if(to_move == std::nullopt) return 1;
        else if( to_move.value() != nullptr){
            insert(*to_move.value());
        }
    }
    return 0;
}



template <typename K, typename V>
int CuckooHashTable<K, V>::grow() {
    size++;
    load_factor = float(size) / (2*capacity);
    if(load_factor > load_factor_max){
        return rehash();
    }
    return 2;
}


template <typename K, typename V>
int CuckooHashTable<K, V>::insert(Pair<K, V> pair) {
    int indeks1 = 0;
    int indeks2 = 0;
    std::optional<Pair<K, V>*> current_pair;
    Pair<K, V>* to_change = &pair;

    for(int i = 0; i < max_steps; i++){
        indeks1 =  hashfunction1(to_change->key);
        current_pair = tab1.get(indeks1);
        if(current_pair == std::nullopt) return 1; // błedny indeks
        if(current_pair.value() == nullptr){ // jeśli nie było elementu to dodaj i wyjdź
            tab1.insert(indeks1, to_change);
            size++;
            return 0;
        }
        tab1.insert(indeks1, to_change);
        to_change = current_pair.value();

        indeks2 = hashfunction2(to_change->key);
        current_pair = tab2.get(indeks2);
        if(current_pair == std::nullopt) return 1; // błedny indeks
        if(current_pair.value() == nullptr){ // jeśli nie było elementu to dodaj i wyjdź
            tab2.insert(indeks2, to_change);
            size++;
            return 0;
        }
        tab2.insert(indeks2, to_change);
        to_change = current_pair.value();
    }
    rehash();
    insert(*to_change); // ponownie wstaw po przechaszowaniu
    return 0;
}


template <typename K, typename V>
int CuckooHashTable<K, V>::remove(int key) {
    return 1;
}


template <typename K, typename V>
int CuckooHashTable<K, V>::display() {
    std::optional<Pair<K, V>*> para;
    for(int i = 0; i < capacity; i++){
        para = tab1.get(i);
        if(para!= std::nullopt){
            if(para.value() != nullptr){
                std::cout << "Indeks: " << i << " | " << *para.value() << std::endl;
            } else{
                std::cout << "Indeks: " << i << " | " << "nullptr"<< std::endl;
            }
        }
    }
    for(int i = 0; i < capacity; i++){
        para = tab2.get(i);
        if(para!= std::nullopt){
            if(para.value() != nullptr){
                std::cout << "Indeks: " << i << " | " << *para.value() << std::endl;
            } else{
                std::cout << "Indeks: " << i << " | " << "nullptr"<< std::endl;
            }
        }
    }
}

template class CuckooHashTable <int, int>;