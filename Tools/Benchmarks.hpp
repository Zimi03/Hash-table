#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <functional>

#include "Results.hpp"
#include "Utils.hpp"

#include "../HashTables/IHashTable.h"
#include "../HashTables/CuckooHashTable.h"
#include "../HashTables/OpenAddressing.hpp"
#include "../HashTables/HashTableArray.h"

#include "../Structures/Pair.hpp"

namespace Benchmark {
    const std::vector<int> TESTING_SIZES = {1024, 2048, 4096, 8192, 12288, 16384, 24576, 32768};
//    const int TESTING_REPETITIONS = 1000;

    std::vector<Results> run(
            std::function<void(IHashTable<int, int>* hash_table, Pair<int, int> testingData)> callback, // funkcja testująca
            std::map<int, Pair<int, int>*> &initzialData, // tablica danych inicjalizujących struktury
            std::map<int, Pair<int, int>*> &testingData, // tablica danych testujących
            const int TESTING_REPETITIONS = 1000
    ) {
        std::vector<Results> benchmarks; // vektor wyników

        std::map<std::string, IHashTable<int, int>*> queues; // mapa: nazwa struktury - struktura
        queues["OpenAddressing"] = new OpenAddressing<int, int>();
        queues["HashTableArray"] = new HashTableArray<int, int>();
        queues["CuckooHashTable"] = new CuckooHashTable<int, int>();

        for (const auto& queue : queues) { // dla obu struktur
            for (const auto& size : TESTING_SIZES) { // dla wszystkich rozmiarów
                Results result(queue.first, size); // inicjalizaja obiektu przechowującego wyniki
                IHashTable<int, int>* copies[TESTING_REPETITIONS]; // pusta tablica kopi struktur

                for (int j = 0; j < size; ++j) { // uzupełnienie inicjalizującej struktury
                    queue.second->insert(initzialData[size][j].key, initzialData[size][j].value);
                }

                if (queue.first == "OpenAddressing"){ // dla open addressing
                    for (int i = 0; i < TESTING_REPETITIONS; ++i){ // stworzenie kopii struktur
                        copies[i] = new OpenAddressing<int, int>(dynamic_cast<OpenAddressing<int, int>*>(queue.second));
                    }
                } else if (queue.first == "HashTableArray") { // dla hash table array
                    for (int i = 0; i < TESTING_REPETITIONS; ++i){ // stowrzenie kopii struktur
                        copies[i] = new HashTableArray<int, int>(dynamic_cast<HashTableArray<int, int>*>(queue.second));
                    }
                } else if (queue.first == "CuckooHashTable") { // dla cucko hash table
                    for (int i = 0; i < TESTING_REPETITIONS; ++i){ // stowrzenie kopii struktur
                        copies[i] = new CuckooHashTable<int, int>(dynamic_cast<CuckooHashTable<int, int>*>(queue.second));
                    }
                } else { // nieznany typ kolejki - exit
                    std::cerr << "Unknown queue type: " << queue.first << std::endl;
                    exit(1);
                }

                for (int i = 0; i < TESTING_REPETITIONS; ++i) { // testy
                    const auto timeStart = std::chrono::high_resolution_clock::now();
                    callback(copies[i], testingData[size][i]);
                    const auto timeEnd = std::chrono::high_resolution_clock::now();
                    const auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(timeEnd - timeStart);
                    result.push(duration.count()); // dodaj wynik do obiektu przechowującego wyniki
                }
                benchmarks.push_back(result); // dodaj obiekt przechowujący wyniki do vektora takich obiektów

                for(int i = 0; i < TESTING_REPETITIONS; i ++){
                    delete copies[i];
                }
            }
        }

        return benchmarks; // zwraca wektor wyników
    }
}
