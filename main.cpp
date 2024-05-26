 #include <iostream>
 #include <vector>
#include "set"
 #include <map>
 #include <functional>
 #include "Tools/Benchmarks.hpp"
 #include "Tools/Utils.hpp"
 #include "Tools/DataExporter.hpp"


 void clearInputStream() {
     std::cin.clear(); // Czyszczenie flag błędów
     std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignorowanie wszystkich pozostałych znaków w buforze
 }
 template<typename T>
 T typeNumber(){
     T i = 0;
     std::cin >> i;
     while (std::cin.fail()) {
         std::cout << "Error! Wrong input data. Try again." << std::endl;
         clearInputStream();
         std::cout << "Type number: ";
         std::cin >> i;
     }
     return i;
 }

/**
 * Metoda wyświetlająca menu
 * @param[out] first  - zwracany przez parametr numer zadania wybrany przez użytkownika
 */
 void menu(unsigned int &first)
{
    std::cout << "\nMENU"  << std::endl;
    std::cout << "0. EXIT." << std::endl;
    std::cout << "1. OpenAdressing" << std::endl;
    std::cout << "2. Array" << std::endl;
    std::cout << "3. CuckooHashing" << std::endl;
     std::cout << "4. Auto-test" << std::endl;


    std::cout << "Choose task: ";
    first = typeNumber<unsigned int>();
}

/**
 * Metoda wyświetlająca menu drugiego poziomu
 * @param[out] second  - zwracany przez parametr numer zadania wybrany przez użytkownika
 * @param firstLevelChoice - wybór z pierwszego poziomu
 * @param size - rozmiar struktury
 */
 void menuSecondLevel(unsigned int firstLevelChoice, unsigned int &second, int size){
    std::string structure = " ";
    if(firstLevelChoice == 1) structure = "OpenAdressing";
    else if(firstLevelChoice == 2) structure = "Array";
    else if(firstLevelChoice == 3) structure = "CuckooHash";
    else if (firstLevelChoice == 4) structure = "Auto-test";

    std::cout << "\n\nMENU - " << structure << " - " << size << std::endl;
    std::cout << "0. Back to main menu." << std::endl;
    std::cout << "1. insert" << std::endl;
    std::cout << "2. remove" << std::endl;
    std::cout << "3. display" << std::endl;

    std::cout << "Choose task: ";
    second = typeNumber<unsigned int>();
}


 void autotest(int repetitions){
    std::map< // Słownik mapujący nazwę testu na funkcję testującą
            std::string,
            std::function<void(IHashTable<int, int> *hash_table, Pair<int, int> testingData)>
    > tests;

    // Funkcja testowa INSERT
    tests["Insert"] = [](IHashTable<int, int> *hash_table, const Pair<int, int>& testingData) {
        hash_table->insert(testingData.key, testingData.value);
    };

    // Funkcja testowa EXTRACT MAX
    tests["Remove"] = [](IHashTable<int, int> *hash_table, const Pair<int, int>& testingData) {
        hash_table->remove(testingData.key);
    };

    // Mapa rozmiar struktury -> tablica danych inicjalizujących
    std::map<
            int, // rozmiar
            Pair<int, int> * // tablica danych
    > initzialData;

//    // Losowanie danych inicjalizujących dla każdego rozmiaru
//    std::cout << "Randoming initzialization data..." << std::endl;
//    for (const auto &size: Benchmark::TESTING_SIZES) { // uzupełnia tablice danych inicjalizujących
//        initzialData[size] = new Pair<int, int>[size]; // alokowanie odpowiedniego rozmiaru tablicy
//        for (int i = 0; i < size; i++) {
//            initzialData[size][i] = Pair<int, int>(size);
//        }

    // Losowanie danych inicjalizujących dla każdego rozmiaru
    for (const auto &size: Benchmark::TESTING_SIZES) {
        initzialData[size] = new Pair<int, int>[size]; // alokowanie odpowiedniego rozmiaru tablicy
        std::set<Pair<int, int>> uniquePairs; // Zestaw do przechowywania unikalnych par

        for (int i = 0; i < size; i++) {
            Pair<int, int> *para;
            do {
                para = new Pair<int, int>(size); // Tworzenie nowej pary
            } while (uniquePairs.find(*para) != uniquePairs.end()); // Sprawdzanie czy para jest unikalna

            uniquePairs.insert(*para); // Dodanie pary do zestawu unikalnych par
            initzialData[size][i] = *para; // Przypisanie pary do tablicy danych testujących
            delete para; // Usunięcie tymczasowej pary, aby uniknąć wycieków pamięci
        }
    }


    // Mapa rozmiar struktury -> tablica danych testujących funkcje insert
    std::map<
            int, // rozmiar
            Pair<int, int> * // tablica danych
    > testingDataInsert;

    // Losowanie danych testowych funkcji insert dla każdego rozmiaru
    std::cout << "Randoming testing data..." << std::endl;
    for (const auto &size: Benchmark::TESTING_SIZES) { // uzupełnia tablice danych testujących - dla INSERT
        testingDataInsert[size] = new Pair<int, int>[repetitions]; // alokowanie odpowiedniego rozmiaru tablicy
        for (int i = 0; i < repetitions; i++) {
            testingDataInsert[size][i] = Pair<int, int>(size);
        }
    }

    // Mapa rozmiar struktury -> tablica danych testujących funkcje modify
    std::map<
            int, // rozmiar
            Pair<int, int> * // tablica danych
    > testingDataRemove;

    // Losowanie danych testowych funkcji remove dla każdego rozmiaru
    for (const auto &size: Benchmark::TESTING_SIZES) { // uzupełnia tablice danych testujących - dla INSERT
        testingDataRemove[size] = new Pair<int, int>[repetitions]; // alokowanie odpowiedniego rozmiaru tablicy
        for (int i = 0; i < repetitions; i++) {
            testingDataRemove[size][i] = Pair<int, int>(initzialData[size], size);
        }
    }

    std::cout << "Randoming data finished." << std::endl << std::endl;

    std::cout << "Testing data..." << std::endl << std::endl;

    DataExporter exporter; // inicjalizuje exporter danych do csv poprzez nazwe testu i wyniki testów
    std::string folderPath = exporter.createNewFolder();
    for (const auto &test: tests) { // dla wszystkich testów
        std::vector<Results> results; // vektor rezultatów
        std::cout << "Method: " << test.first << "..." << std::endl;
        if (test.first == "Remove") {
            results = Benchmark::run(test.second, initzialData, testingDataRemove, repetitions); // funkcja testująca
        } else {
            results = Benchmark::run(test.second, initzialData, testingDataInsert, repetitions); // funkcja testująca
        }

        for (auto &benchmark: results) { // dla rezultatów
            float average = benchmark.average(); // pobiera średnią z pomiarów
            std::cout << "Class: " << benchmark.className; // nazwa struktury
            std::cout << " Size: " << benchmark.stack; // wielkość struktury
            std::cout << " Average: " << average << " ns"; // średnia
            std::cout << std::endl;
        }

        exporter = DataExporter(test.first, results); // inicjalizuje exporter danych do csv poprzez nazwe testu i wyniki testów
        exporter.generate(folderPath); // zapisuje dane

        std::cout << std::endl;
    }

}


 int main(int argc, char* argv[]) {
    unsigned int firstChocie = 0, secondChoice = 0;
    int size, key, value, new_priority;
    int info;
    std::optional<Pair<int,int>> opt_val;

    OpenAddressing<int, int>* hashTableOpenAddressing = nullptr;
    CuckooHashTable<int, int>* hashTableCuckoo = nullptr;
    HashTableArray<int, int>*  hashTableArray = nullptr;

    do {
        menu(firstChocie);
        switch (firstChocie) {
            case 0:
                std::cout << "EXITING";
                break;
                /*  HashTable - OpenAddressing  */
            case 1: // HashTable - OpenAddressing
                std::cout << "Structure size: " ;
                size = typeNumber<int>();
                hashTableOpenAddressing = new OpenAddressing<int, int>;
                for(int i = 0; i < size; i++){
                    do{
                        info = hashTableOpenAddressing->insert(Utils::generateNumber(size * 5), Utils::generateNumber(size));
                    } while(info);
                }

                do {
                    menuSecondLevel(firstChocie, secondChoice, size);
                    switch (secondChoice) {
                        case 0:
                            std::cout << "Quiting from OpenAddressing - hash table" << std::endl;
                            delete hashTableOpenAddressing;
                            break;
                        case 1: // insert
                            std::cout << "Insert" << std::endl;
                            std::cout << "Key: " ;
                            key = typeNumber<int>();
                            std::cout << "Value: " ;
                            value = typeNumber<int>();
                            info = hashTableOpenAddressing->insert(key, value);
                            if(!info){
                                std::cout << "Inserted: " << Pair<int, int>(key, value) << std::endl;
                            } else std::cout << "Already exists such key: " << key << std::endl;
                            break;
                        case 2: // remove
                            std::cout << "Remove" << std::endl;
                            std::cout << "Key: " ;
                            key = typeNumber<int>();
                            info = hashTableOpenAddressing->remove(key);
                            if(!info){
                                std::cout << "Removed key: " << key << std::endl;
                            } else std::cout << "Not found key: " << key << std::endl;
                            break;
                        case 3:
                            std::cout << "Display" << std::endl;
                            hashTableOpenAddressing->display();
                            break;
                        default:
                            std::cout << "Wrong choice. Try again." << std::endl;
                            break;

                    }
                } while (secondChoice != 0);
                break;
            case 2: // HashTable - Array
                std::cout << "Structure size: " ;
                size = typeNumber<int>();
                hashTableArray = new HashTableArray<int, int>;
                for(int i = 0; i < size; i++) {
                    do {
                        info = hashTableArray->insert(Utils::generateNumber(size * 5), Utils::generateNumber(size));
                    } while (info);
                }

                do {
                    menuSecondLevel(firstChocie, secondChoice, size);
                    switch (secondChoice) {
                        case 0:
                            std::cout << "Quiting from Array - hash table" << std::endl;
                            delete hashTableArray;
                            break;
                        case 1: // insert
                            std::cout << "Insert" << std::endl;
                            std::cout << "Key: " ;
                            key = typeNumber<int>();
                            std::cout << "Value: " ;
                            value = typeNumber<int>();
                            info = hashTableArray->insert(key, value);
                            if(!info){
                                std::cout << "Inserted: " << Pair<int, int>(key, value) << std::endl;
                            } else std::cout << "Already exists such key: " << key << std::endl;
                            break;
                        case 2: // remove
                            std::cout << "Remove" << std::endl;
                            std::cout << "Key: " ;
                            key = typeNumber<int>();
                            info = hashTableArray->remove(key);
                            if(!info){
                                std::cout << "Removed key: " << key << std::endl;
                            } else std::cout << "Not found key: " << key << std::endl;
                            break;
                        case 3:
                            std::cout << "Display" << std::endl;
                            hashTableArray->display();
                            break;
                        default:
                            std::cout << "Wrong choice. Try again." << std::endl;
                            break;
                    }
                } while (secondChoice != 0);
                break;
            case 3: // HashTable - Cuckoo
                std::cout << "Structure size: " ;
                size = typeNumber<int>();
                hashTableCuckoo = new CuckooHashTable<int, int>;
                for(int i = 0; i < size; i++) {
                    do {
                        info = hashTableCuckoo->insert(Utils::generateNumber(size * 5), Utils::generateNumber(size));
                    } while (info);
                }

                do {
                    menuSecondLevel(firstChocie, secondChoice, size);
                    switch (secondChoice) {
                        case 0:
                            std::cout << "Quiting from Cuckoo hashing - hash table" << std::endl;
                            delete hashTableCuckoo;
                            break;
                        case 1: // insert
                            std::cout << "Insert" << std::endl;
                            std::cout << "Key: " ;
                            key = typeNumber<int>();
                            std::cout << "Value: " ;
                            value = typeNumber<int>();
                            info = hashTableCuckoo->insert(key, value);
                            if(!info){
                                std::cout << "Inserted: " << Pair<int, int>(key, value) << std::endl;
                            } else std::cout << "Already exists such key: " << key << std::endl;
                            break;
                        case 2: // remove
                            std::cout << "Remove" << std::endl;
                            std::cout << "Key: " ;
                            key = typeNumber<int>();
                            info = hashTableCuckoo->remove(key);
                            if(!info){
                                std::cout << "Removed key: " << key << std::endl;
                            } else std::cout << "Not found key: " << key << std::endl;
                            break;
                        case 3:
                            std::cout << "Display" << std::endl;
                            hashTableCuckoo->display();
                            break;
                        default:
                            std::cout << "Wrong choice. Try again." << std::endl;
                            break;
                    }
                } while (secondChoice != 0);
                break;
                /*  AUTO TEST   */
            case 4:
                std::cout << std::endl << "AUTO-TEST" << std::endl;
                std::cout << "Repetitions: " ;
                size = typeNumber<int>();
                autotest(size);
                break;
            default:
                std::cout << "Wrong choice. Try again." << std::endl;
                break;
        }
    } while (firstChocie != 0);
    exit(0);
}
