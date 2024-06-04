//
// Created by grzegorz on 23.05.24.
//

#include "CuckooHashTable.h"


template <typename K, typename V>
CuckooHashTable<K, V>::CuckooHashTable(int max_steps_) : max_steps(max_steps_){
    tab1 = new Pair<K, V>*[capacity] {nullptr};
    tab2 = new Pair<K, V>*[capacity] {nullptr};
}

template <typename K, typename V>
CuckooHashTable<K, V>::CuckooHashTable() {
    tab1 = new Pair<K, V>*[capacity] {nullptr};
    tab2 = new Pair<K, V>*[capacity] {nullptr};
}

/**
 * Copy constructor
 * @tparam K
 * @tparam V
 * @param to_copy
 */
template <typename K, typename V>
CuckooHashTable<K, V>::CuckooHashTable(CuckooHashTable<K, V> *to_copy) {
    capacity = to_copy->capacity;
    size = to_copy->size;
    load_factor = to_copy->load_factor;
    load_factor_max = to_copy->load_factor_max;
    max_steps = to_copy->max_steps;

    Pair<K, V> *tmp;
    tab1 = new Pair<K, V>*[capacity] {nullptr};
    tab2 = new Pair<K, V>*[capacity] {nullptr};
    for(int i =0 ; i < capacity; i++){
//        tab1[i] = to_copy->tab1[i] ? new Pair<K, V>(*to_copy->tab1[i]) : nullptr;
//        tab2[i] = to_copy->tab2[i] ? new Pair<K, V>(*to_copy->tab2[i]) : nullptr;
        tmp = to_copy->tab1[i];
        if(tmp != nullptr) {
            tab1[i] = new Pair<K, V>(tmp->key, tmp->value);
        }
        tmp = to_copy->tab2[i];
        if(tmp != nullptr) {
            tab2[i] = new Pair<K, V>(tmp->key, tmp->value);
        }
    }
}

template <typename K, typename V>
CuckooHashTable<K, V>::~CuckooHashTable() {
    for (int i = 0; i < capacity; i++) {
        if (tab1[i] != nullptr) delete tab1[i];
        if (tab2[i] != nullptr) delete tab2[i];
    }
    delete[] tab1;
    delete[] tab2;
}

/**
 * Hash function 1 returning int as a hashed key
 * @param key
 * @return
 */
template <typename K, typename V>
int CuckooHashTable<K, V>::hashfunction1(K key){
    int x = key % capacity;
    return x;
}

/**
 * Hash function 2 returning int as a hashed key
 * @param key
 * @return
 */
template <typename K, typename V>
int CuckooHashTable<K, V>::hashfunction2(K key){
    int x = (key/capacity) % capacity;
    return x;
}

/**
 * Rehashes whole hash table with new doubled capacity
 * @tparam K
 * @tparam V
 * @return
 */
template <typename K, typename V>
int CuckooHashTable<K, V>::rehash() {
    int old_capacity = capacity;
    capacity*=2;
    Pair<K, V>** tmp1 = tab1;
    Pair<K, V>** tmp2 = tab2;
    Pair<K, V>* to_move;
    tab1 = new Pair<K, V>*[capacity] {nullptr};
    tab2 = new Pair<K, V>*[capacity] {nullptr};
    size = 0;

    for(int i = 0; i < old_capacity; i ++) { // każdy element ze starej tablicy dodaje do nowej powiększone
        to_move = tmp1[i];
        if (to_move != nullptr) {
            insert(to_move->key, to_move->value);
            delete tmp1[i];
        }

        to_move = tmp2[i];
        if( to_move != nullptr){
            insert(to_move->key, to_move->value);
            delete tmp2[i];
        }
    }

    delete [] tmp1;
    delete [] tmp2;
    return 0;
}

/**
 * Increase size by one
 * Checks whether there is a need of increasing size of hash table
 * If there is, it rehash hash table (rehash function doubles capacity)
 * @tparam K
 * @tparam V
 * @return
 */
template <typename K, typename V>
int CuckooHashTable<K, V>::grow() {
    size++;
    load_factor = float(size) / (2*capacity);
    if(load_factor > load_factor_max)
        return rehash();
    return 2;
}

template <typename K, typename V>
int CuckooHashTable<K, V>::insert(K key, V value) {

    int indeks1 = hashfunction1(key);
    int indeks2 = hashfunction2(key);

    // sprawdzenie czy już jest element o takim kluczu
    if(tab1[indeks1] != nullptr && tab1[indeks1]->key != key) return 1;
    if(tab2[indeks2] != nullptr && tab2[indeks2]->key != key) return 1;
    Pair<K, V>* current_pair;
    Pair<K, V>* to_change = new Pair<K, V>(key, value);

    for(int i = 0; i < max_steps; i++){ // wykona się maksymalnie max_steps razy
        indeks1 =  hashfunction1(to_change->key);
        current_pair = tab1[indeks1];
        if (current_pair == nullptr) { // jeśli kubełek pusty to dodaj i wyjdź
            tab1[indeks1] = to_change;
            grow();
            return 0;
        }
        tab1[indeks1] = to_change; // jeśli kubełek zajęty to podmień i przejdź dalej
        to_change = current_pair;

        indeks2 = hashfunction2(to_change->key);
        current_pair = tab2[indeks2];
        if(current_pair == nullptr){ // jeśli nie było elementu to dodaj i wyjdź
            tab2[indeks2] = to_change;
            grow();
            return 0;
        }
        tab2[indeks2] = to_change; // jeśli kubełek zajęty to podmień i przejdź dalej
        to_change = current_pair;
    }
    rehash(); // jeśli nie udało się znaleźć pustego kubełka to wykonaj rehashowanie ze zwiększeniem pojemności
    insert(to_change->key, to_change->value); // ponownie wstaw po przehaszowaniu
    delete to_change;
    return 0;
}


template <typename K, typename V>
int CuckooHashTable<K, V>::remove(int key) {
    int indeks1 = hashfunction1(key);
    if (tab1[indeks1] != nullptr && tab1[indeks1]->key == key) { // jeśli kubełek1 niepusty i klucz się zgadza
        delete tab1[indeks1]; // usuń
        tab1[indeks1] = nullptr;
        size--;
        return 0;
    }

    int indeks2 = hashfunction2(key);
    if (tab2[indeks2] != nullptr && tab2[indeks2]->key == key) { // jeśli kubełek2 niepusty i klucz się zgadza
        delete tab2[indeks2]; // usuń
        tab2[indeks2] = nullptr;
        size--;
        return 0;
    }
    return 1; // Element nie znaleziony
}

/**
 * Displays hash table
 */
template <typename K, typename V>
void CuckooHashTable<K, V>::display() {
    Pair<K, V>* para;
    for(int i = 0; i < capacity; i++){
        para = tab1[i];
        if(para != nullptr) std::cout << "Index: " << i << " | " << *para << std::endl;
        else std::cout << "Indeks: " << i << " | " << "empty"<< std::endl;
    }
    for(int i = 0; i < capacity; i++){
        para = tab2[i];
        if(para != nullptr) std::cout << "Index: " << i << " | " << *para << std::endl;
        else std::cout << "Index: " << i << " | " << "empty"<< std::endl;
    }
}


template class CuckooHashTable <int, int>;