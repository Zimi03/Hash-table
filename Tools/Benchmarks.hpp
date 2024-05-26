#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <functional>
#include <memory>

#include "Results.hpp"
#include "Utils.hpp"

#include "../HashTables/IHashTable.h"
#include "../HashTables/CuckooHashTable.h"
#include "../HashTables/OpenAddressing.hpp"
#include "../HashTables/HashTableArray.h"

#include "../Structures/Pair.hpp"


namespace Benchmark {
    const std::vector<int> TESTING_SIZES = {1024, 2048, 4096, 8192, 12288, 16384, 24576, 32768};

    std::vector<Results> run(
            std::function<void(IHashTable<int, int>* hash_table, Pair<int, int> testingData)> callback, // funkcja testująca
            std::map<int, Pair<int, int>*> &initzialData, // tablica danych inicjalizujących struktury
            std::map<int, Pair<int, int>*> &testingData, // tablica danych testujących
            const int TESTING_REPETITIONS = 1000
    ) {
        std::vector<Results> benchmarks; // vektor wyników

        std::map<std::string, IHashTable<int, int>*> hashTables; // mapa: nazwa struktury - struktura
        hashTables["OpenAddressing"] = new OpenAddressing<int, int>();
        hashTables["HashTableArray"] = new HashTableArray<int, int>();
        hashTables["CuckooHashTable"] = new CuckooHashTable<int, int>();

        for (const auto& hashTable : hashTables) { // dla obu struktur
            std::cout << "Testing: " << hashTable.first << std::endl;
            for (const auto& size : TESTING_SIZES) { // dla wszystkich rozmiarów
                std::cout << "Size: " << size << " initzializaton... ";
                Results result(hashTable.first, size); // inicjalizaja obiektu przechowującego wyniki
                IHashTable<int, int>* copies[TESTING_REPETITIONS]; // pusta tablica kopi struktur

                for (int j = 0; j < size; ++j) { // uzupełnienie inicjalizującej struktury
                    hashTable.second->insert(initzialData[size][j].key, initzialData[size][j].value);
                }

                std::cout << " copying...";
                if (hashTable.first == "OpenAddressing"){ // dla open addressing
                    for (int i = 0; i < TESTING_REPETITIONS; ++i){ // stworzenie kopii struktur
                        copies[i] = new OpenAddressing<int, int>(dynamic_cast<OpenAddressing<int, int>*>(hashTable.second));
                    }
                } else if (hashTable.first == "HashTableArray") { // dla hash table array
                    for (int i = 0; i < TESTING_REPETITIONS; ++i){ // stowrzenie kopii struktur
                        copies[i] = new HashTableArray<int, int>(dynamic_cast<HashTableArray<int, int>*>(hashTable.second));
                    }
                } else if (hashTable.first == "CuckooHashTable") { // dla cucko hash table
                    for (int i = 0; i < TESTING_REPETITIONS; ++i){ // stowrzenie kopii struktur
                        copies[i] = new CuckooHashTable<int, int>(dynamic_cast<CuckooHashTable<int, int>*>(hashTable.second));
                    }
                } else { // nieznany typ kolejki - exit
                    std::cerr << "Unknown hashTable type: " << hashTable.first << std::endl;
                    exit(1);
                }
                std::cout << " testing...";
                for (int i = 0; i < TESTING_REPETITIONS; ++i) { // testy
                    const auto timeStart = std::chrono::high_resolution_clock::now();
                    callback(copies[i], testingData[size][i]);
                    const auto timeEnd = std::chrono::high_resolution_clock::now();
                    const auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(timeEnd - timeStart);
                    result.push(duration.count()); // dodaj wynik do obiektu przechowującego wyniki
                }
                benchmarks.push_back(result); // dodaj obiekt przechowujący wyniki do vektora takich obiektów

                std::cout << " deleting copies..." << std::endl;
                for(int i = 0; i < TESTING_REPETITIONS; i ++){
                    delete copies[i];
                }

                if (hashTable.first == "OpenAddressing"){ // dla open addressing
                    // delete hashTable.second;
                    delete hashTables["OpenAddressing"];
                    hashTables["OpenAddressing"] = new OpenAddressing<int, int>();
                } else if (hashTable.first == "HashTableArray") { // dla hash table array
                    // delete hashTable.second;
                    delete hashTables["HashTableArray"];
                    hashTables["HashTableArray"] = new HashTableArray<int, int>();
                } else if (hashTable.first == "CuckooHashTable") { // dla cucko hash table
                    // delete hashTable.second;
                    delete hashTables["CuckooHashTable"];
                    hashTables["CuckooHashTable"] = new CuckooHashTable<int, int>();
                } else { // nieznany typ kolejki - exit
                    std::cerr << "Unknown hashTable type: " << hashTable.first << std::endl;
                    exit(1);
                }
            }
        }

        delete hashTables["OpenAddressing"];
        delete hashTables["HashTableArray"];
        delete hashTables["CuckooHashTable"];
        return benchmarks; // zwraca wektor wyników
    }
}





//
//namespace Benchmark {
//    const std::vector<int> TESTING_SIZES = {1024, 2048, 4096, 8192, 12288, 16384, 24576, 32768};
//
//    std::vector<Results> run(
//            std::function<void(IHashTable<int, int>* hash_table, Pair<int, int> testingData)> callback,
//            std::map<int, Pair<int, int>*> &initzialData,
//            std::map<int, Pair<int, int>*> &testingData,
//            const int TESTING_REPETITIONS
//    ) {
//        std::vector<Results> benchmarks;
//
//        std::map<std::string, std::unique_ptr<IHashTable<int, int>>> hashTables;
////        hashTables["OpenAddressing"] = std::make_unique<OpenAddressing<int, int>>();
////        hashTables["HashTableArray"] = std::make_unique<HashTableArray<int, int>>();
//        hashTables["CuckooHashTable"] = std::make_unique<CuckooHashTable<int, int>>();
//
//        for (const auto& hashTableEntry : hashTables) {
//            const auto& hashTableName = hashTableEntry.first;
//            const auto& hashTable = hashTableEntry.second;
//
//            std::cout << "Testing: " << hashTableName << std::endl;
//
//            for (const auto& size : TESTING_SIZES) {
//                std::cout << "Size: " << size << " initialization... ";
//                Results result(hashTableName, size);
//                std::vector<std::unique_ptr<IHashTable<int, int>>> copies;
//
//                for (int j = 0; j < size; ++j) {
//                    hashTable->insert(initzialData[size][j].key, initzialData[size][j].value);
//                }
//
//                std::cout << " copying...";
//                for (int i = 0; i < TESTING_REPETITIONS; ++i) {
//                    if (hashTableName == "OpenAddressing") {
//                        copies.push_back(std::make_unique<OpenAddressing<int, int>>(dynamic_cast<OpenAddressing<int, int>*>(hashTable.get())));
//                    } else if (hashTableName == "HashTableArray") {
//                        copies.push_back(std::make_unique<HashTableArray<int, int>>(dynamic_cast<HashTableArray<int, int>*>(hashTable.get())));
//                    } else if (hashTableName == "CuckooHashTable") {
//                        copies.push_back(std::make_unique<CuckooHashTable<int, int>>(dynamic_cast<CuckooHashTable<int, int>*>(hashTable.get())));
//                    } else {
//                        std::cerr << "Unknown hashTable type: " << hashTableName << std::endl;
//                        exit(1);
//                    }
//                }
//
//                std::cout << " testing...";
//                for (int i = 0; i < TESTING_REPETITIONS; ++i) {
//                    const auto timeStart = std::chrono::high_resolution_clock::now();
//                    callback(copies[i].get(), testingData[size][i]);
//                    const auto timeEnd = std::chrono::high_resolution_clock::now();
//                    const auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(timeEnd - timeStart);
//                    result.push(duration.count());
//                }
//                benchmarks.push_back(result);
//
//                std::cout << " deleting copies..." << std::endl;
//                copies.clear(); // Wszystkie kopie zostaną automatycznie usunięte
//            }
//        }
//
//        return benchmarks;
//    }
//}





