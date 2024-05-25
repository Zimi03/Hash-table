#ifndef OPENADDRESSING_HPP
#define OPENADDRESSING_HPP

#include <IHashTable.h>

template <typename K, typename  V>
class OpenAddressing: public IHashTable<K, V> {
    Pair<K, V>** arr;
    int capacity;
    int curr_size;
    float load_factor;
    float load_factor_max;

    int hash(K key) {
        int x = key % capacity;
        return x;
    }

    void grow() {
        int old_capacity = capacity;
        capacity *= 2;
        Pair<K, V>** tmp = arr;
        arr = new Pair<K, V>*[capacity] {nullptr};
        curr_size = 0;

        for (int i = 0; i < old_capacity; i++) {
            if (tmp[i] == nullptr) continue;
            int index = hash(tmp[i]->key);

            while (arr[index] != nullptr) {
                index++;
                if (index == capacity) index = 0;
            }

            arr[index] = new Pair<K, V>(tmp[i]->key, tmp[i]->value);
            curr_size++;
        }

        for (int i = 0; i < old_capacity; i++) {
            if (tmp[i] != nullptr) delete tmp[i];
        }
        delete[] tmp;
        
    }

public:
    OpenAddressing() {
        capacity = 10;
        curr_size = 0;
        load_factor = 0;
        load_factor_max = 0.7;
        arr = new Pair<K, V>*[capacity] {nullptr};
    }

    
    explicit OpenAddressing(OpenAddressing<K, V> *to_copy) {
        capacity = to_copy->capacity;
        curr_size = to_copy->curr_size;
        load_factor = to_copy->load_factor;
        load_factor_max = to_copy->load_factor_max;
        arr = new Pair<K, V>*[capacity] {nullptr};

        for (int i = 0; i < capacity; i++) {
            if (to_copy->arr[i] != nullptr) {
                arr[i] = new Pair<K, V>(to_copy->arr[i]->key, to_copy->arr[i]->value);
            }
        }
    }

    
    ~OpenAddressing() override {
        for (int i = 0; i < capacity; i++) {
            if (arr[i] != nullptr) delete arr[i];
        }
        delete[] arr;
    }

    int insert(K key, V value) override {
        int index = hash(key);
        while (arr[index] != nullptr && arr[index]->key != key) {
            index++;
            if (index == capacity) index = 0;
        }
        if (arr[index] != nullptr) return 1;

        arr[index] = new Pair<K, V>(key, value);
        curr_size++;
        load_factor = static_cast<float>(curr_size) / capacity;

        if (load_factor > load_factor_max) grow();

        return 0;
    }

    int remove(int key) override {
        int index = hash(key);
        while (arr[index] != nullptr && arr[index]->key != key) {
            index++;
            if (index == capacity) index = 0;
        }
        if (arr[index] == nullptr) return 1;

        delete arr[index];
        arr[index] = nullptr;
        curr_size--;
        load_factor = static_cast<float>(curr_size) / capacity;

        return 0;
    }

    void display() {
        for (int i = 0; i < capacity; i++) {
            if (arr[i] != nullptr) {
                std::cout << "Key: " << arr[i]->key << " Value: " << arr[i]->value << std::endl;
            } else {
                std::cout << "Index: " << i << "Empty" << std::endl;
            }
        }
    }
};

#endif //OPENADDRESSING_HPP
