//
// Created by grzegorz on 23.05.24.
//

#ifndef HASH_TABLES_PAIR_HPP
#define HASH_TABLES_PAIR_HPP

template <typename K, typename V>
struct  Pair {
public:
    K key;
    V value;

    Pair(Pair &para){
        key = para.key;
        value = para.value;
    };
    Pair(K key_, V val) : key(key_), value(val) {};
    Pair() = default;
    ~Pair() = default;


    friend std::ostream& operator<<(std::ostream& os, const Pair<K, V>& p) {
        os << "{" << p.key << ", " << p.value << "}";
        return os;
    }
};

#endif //HASH_TABLES_PAIR_HPP
