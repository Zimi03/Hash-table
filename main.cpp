#include "../HashTables/CuckooHashTable.h"
#include "../HashTables/HashTableArray.h"
#include "../Tools/Utils.hpp"





int main(){

//    CuckooHashTable<int, int> hash_table;
    HashTableArray<int, int> hash_table;
    int size = 20;
    Pair<int, int>* para;
    std::cout << "\nSTART\n";
    Pair<int, int> array[size];

    for(int i = 0; i < size; i++){
        std::cout << i <<"\n";
        array[i] = Pair<int, int>(Utils::generateNumber(100),i);
        hash_table.insert(array[i].key, array[i].value);
    }
    HashTableArray<int, int> hash_table2(hash_table);
//    CuckooHashTable<int, int> hash_table2(hash_table);


    hash_table2.display();
    for(int i = 0; i < size; i++){
        std::cout << "REMOVE " << array[i];
        int index = hash_table2.remove(array[i].key);
        if(index == 0) std::cout << " - Usunięto " << std::endl;
//        if(index != -1) std::cout << " - Usunięto - indeks: " << index << std::endl;
        else std::cout << " - NIE usunięto" << std::endl;
    }

    hash_table2.display();
    hash_table.display();


}
















//#include <iostream>
//#include <vector>
//#include <map>
//#include <functional>
//#include "Tools/Benchmarks.hpp"
//#include "Tools/Utils.hpp"
//#include "Tools/DataExporter.hpp"
//
//void clearInputStream() {
//    std::cin.clear(); // Czyszczenie flag błędów
//    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignorowanie wszystkich pozostałych znaków w buforze
//}
//template<typename T>
//T typeNumber(){
//    T i = 0;
//    std::cin >> i;
//    while (std::cin.fail()) {
//        std::cout << "Error! Wrong input data. Try again." << std::endl;
//        clearInputStream();
//        std::cout << "Type number: ";
//        std::cin >> i;
//    }
//    return i;
//}
//
///**
// * Metoda wyświetlająca menu
// * @param[out] zadanie  - zwracany przez parametr numer zadania wybrany przez użytkownika
// */
//void menu(unsigned int &first)
//{
//    std::cout << "\nMENU"  << std::endl;
//    std::cout << "0. EXIT." << std::endl;
//    std::cout << "1. PriorityQueue - heap" << std::endl;
//    std::cout << "2. PriorityQueue - dynamic array" << std::endl;
//    std::cout << "3. Auto-test" << std::endl;
//
//
//    std::cout << "Choose task: ";
//    first = typeNumber<unsigned int>();
//}
//
//
//void menuSecondLevel(unsigned int firstLevelChoice, unsigned int &second, int size){
//    std::string structure = " ";
//    if(firstLevelChoice == 1) structure = "PriorityQueue - heap";
//    else if(firstLevelChoice == 2)  structure = "PriorityQueue - dynamic array";
//    else if(firstLevelChoice == 3)  structure = "Auto-test";
//
//    std::cout << "\n\nMENU - " << structure << " - " << size << std::endl;
//    std::cout << "0. Back to main menu." << std::endl;
//    std::cout << "1. insert" << std::endl;
//    std::cout << "2. extractMax" << std::endl;
//    std::cout << "3. peek" << std::endl;
//    std::cout << "4. modifyKey" << std::endl;
//    std::cout << "5. return size" << std::endl;
//    std::cout << "6. display" << std::endl;
//
//    std::cout << "Choose task: ";
//    second = typeNumber<unsigned int>();
//}
//
//void autotest(int repetitions){
//    std::map< // Słownik mapujący nazwę testu na funkcję testującą
//            std::string,
//            std::function<void(IPriorityQueue *queue, TestingValue testingData)>
//    > tests;
//
//    // Funkcja testowa INSERT
//    tests["Insert"] = [](IPriorityQueue *queue, TestingValue testingData) {
//        queue->insert(testingData);
//    };
//
//    // Funkcja testowa EXTRACT MAX
//    tests["ExtractMax"] = [](IPriorityQueue *queue, TestingValue testingData) {
//        queue->extractMax();
//    };
//
//    // Funkcja testowa PEEK
//    tests["Peek"] = [](IPriorityQueue *queue, TestingValue testingData) {
//        queue->peek();
//    };
//
//    // Funkcja testowa MODIFY KEY
//    tests["ModifyKey"] = [](IPriorityQueue *queue, TestingValue testingData) {
//        queue->modifyKey(testingData, testingData.newPriority);
//    };
//
//    // Funkcja testowa RETRUN SIZE
//    tests["ReturnSize"] = [](IPriorityQueue *queue, TestingValue testingData) {
//        queue->getSize();
//    };
//
//    // Mapa rozmiar struktury -> tablica danych inicjalizujących
//    std::map<
//            int, // rozmiar
//            PriorityValue * // tablica danych
//    > initzialData;
//
//    // Losowanie danych inicjalizujących dla każdego rozmiaru
//    std::cout << "Randoming initzialization data..." << std::endl;
//    for (const auto &data: Benchmark::TESTING_SIZES) { // uzupełnia tablice danych inicjalizujących
//        initzialData[data] = new PriorityValue[data]; // alokowanie odpowiedniego rozmiaru tablicy
//        for (int i = 0; i < data; i++) {
//            initzialData[data][i] = TestingValue(data);
////            initzialData[data][i].value = Utils::generateNumber(data);
////            initzialData[data][i].priority = Utils::generateNumber(data*3);
//        }
//    }
//
//    // Mapa rozmiar struktury -> tablica danych testujących funkcje insert
//    std::map<
//            int, // rozmiar
//            TestingValue * // tablica danych
//    > testingDataInsert;
//
//    // Losowanie danych testowych funkcji insert dla każdego rozmiaru
//    std::cout << "Randoming testing data..." << std::endl;
//    for (const auto &data: Benchmark::TESTING_SIZES) { // uzupełnia tablice danych testujących - dla INSERT
//        testingDataInsert[data] = new TestingValue[data]; // alokowanie odpowiedniego rozmiaru tablicy
//        for (int i = 0; i < data; i++) {
//            testingDataInsert[data][i] = TestingValue(data);
//        }
//    }
//
//    // Mapa rozmiar struktury -> tablica danych testujących funkcje modify
//    std::map<
//            int, // rozmiar
//            TestingValue * // tablica danych
//    > testingDataModify;
//
//    // Losowanie danych testowych funkcji modify dla każdego rozmiaru
//    for (const auto &data: Benchmark::TESTING_SIZES) { // uzupełnia tablice danych testujących - dla INSERT
//        testingDataModify[data] = new TestingValue[data]; // alokowanie odpowiedniego rozmiaru tablicy
//        for (int i = 0; i < data; i++) {
//            testingDataModify[data][i] = TestingValue(initzialData[data], data);
//        }
//    }
//
//    std::cout << "Randoming data finished." << std::endl << std::endl;
//
//    std::cout << "Testing data..." << std::endl << std::endl;
//
//    DataExporter exporter; // inicjalizuje exporter danych do csv poprzez nazwe testu i wyniki testów
//    std::string folderPath = exporter.createNewFolder();
//    for (const auto &test: tests) { // dla wszystkich testów
//        std::vector<Results> results; // vektor rezultatów
//        std::cout << "Method: " << test.first << "..." << std::endl;
//        if (test.first == "ModifyKey") {
//            results = Benchmark::run(test.second, initzialData, testingDataModify, repetitions); // funkcja testująca
//        } else {
//            results = Benchmark::run(test.second, initzialData, testingDataInsert, repetitions); // funkcja testująca
//        }
//
//        for (auto &benchmark: results) { // dla rezultatów
//            float average = benchmark.average(); // pobiera średnią z pomiarów
//            std::cout << "Class: " << benchmark.className; // nazwa struktury
//            std::cout << " Size: " << benchmark.stack; // wielkość struktury
//            std::cout << " Average: " << average << " ns"; // średnia
//            std::cout << std::endl;
//        }
//
//        exporter = DataExporter(test.first, results); // inicjalizuje exporter danych do csv poprzez nazwe testu i wyniki testów
//        exporter.generate(folderPath); // zapisuje dane
//
//        std::cout << std::endl;
//    }
//
//}
//
//
//int main(int argc, char* argv[]) {
//    unsigned int firstChocie = 0, secondChoice = 0;
//    int size, priority, value, new_priority;
//    std::optional<PriorityValue> opt_val;
//
//    PriorityQueueHeap* pqHeap = nullptr;
//    PriorityQueueArray* pqArray = nullptr;
//
//    do {
//        menu(firstChocie);
//        switch (firstChocie) {
//            case 0:
//                std::cout << "EXITING";
//                break;
//                /*  PriorityQueue - heap   */
//            case 1: // PriorityQueue - heap
//                std::cout << "Structure size: " ;
//                size = typeNumber<int>();
//                pqHeap = new PriorityQueueHeap;
//                for(int i = 0; i < size; i++){
//                    pqHeap->insert(PriorityValue(Utils::generateNumber(size*5),Utils::generateNumber(size)));
//                }
//
//                do {
//                    menuSecondLevel(firstChocie, secondChoice, size);
//                    switch (secondChoice) {
//                        case 0:
//                            std::cout << "Quiting from PriorityQueue - heap" << std::endl;
//                            delete pqHeap;
//                            break;
//                        case 1: // insert
//                            std::cout << "Insert" << std::endl;
//                            std::cout << "Priority: " ;
//                            priority = typeNumber<int>();
//                            std::cout << "Value: " ;
//                            value = typeNumber<int>();
//                            pqHeap->insert(PriorityValue(priority,value));
//                            break;
//                        case 2: // extractMax
//                            std::cout << "ExtractMax" << std::endl;
//                            opt_val = pqHeap->extractMax();
//                            if(opt_val != std::nullopt){
//                                std::cout << "Extracted value: " << opt_val.value() << std::endl;
//                            } else std::cout << "Empty structure" << std::endl;
//                            break;
//                        case 3: // peek
//                            std::cout << "Peek" << std::endl;
//                            opt_val = pqHeap->peek();
//                            if(opt_val != std::nullopt){
//                                std::cout << "Peeked value: " << opt_val.value() << std::endl;
//                            } else std::cout << "Empty structure" << std::endl;
//                            break;
//                        case 4: // modifyKey
//                            std::cout << "ModifyKey" << std::endl;
//                            std::cout << "Old priority: " ;
//                            priority = typeNumber<int>();
//                            std::cout << "Value: " ;
//                            value = typeNumber<int>();
//                            std::cout << "New priority: " ;
//                            new_priority = typeNumber<int>();
//                            value = pqHeap->modifyKey(PriorityValue(priority,value), new_priority);
//                            if(value == 0) std::cout << "Success" << std::endl;
//                            else if (value == 1) std::cout << "The same priority" << std::endl;
//                            else if (value == 2) std::cout << "No such element" << std::endl;
//                            else if (value == 3) std::cout << "New priority less than zero" << std::endl;
//                            break;
//                        case 5: // display
//                            std::cout << "RerurnSize" << std::endl;
//                            std::cout << "Size: " << pqHeap->getSize() << std::endl;
//                            break;
//                        case 6: // display
//                            pqHeap->display();
//                            break;
//                        default:
//                            std::cout << "Wrong choice. Try again." << std::endl;
//                            break;
//                    }
//                } while (secondChoice != 0);
//                break;
//                /*  PriorityQueue - dynamic array   */
//            case 2: // PriorityQueue - dynamic array
//                std::cout << "Structure size: " ;
//                size = typeNumber<int>();
//                pqArray = new PriorityQueueArray;
//                for(int i = 0; i < size; i++){
//                    pqArray->insert(PriorityValue(Utils::generateNumber(size*5),Utils::generateNumber(size)));
//                }
//                do {
//                    menuSecondLevel(firstChocie, secondChoice, size);
//                    switch (secondChoice) {
//                        case 0:
//                            std::cout << "Quiting from PriorityQueue - dynamic array" << std::endl;
//                            delete pqArray;
//                            break;
//                        case 1: // insert
//                            std::cout << "Insert" << std::endl;
//                            std::cout << "Priority: " ;
//                            priority = typeNumber<int>();
//                            std::cout << "Value: " ;
//                            value = typeNumber<int>();
//                            pqArray->insert(PriorityValue(priority,value));
//                            break;
//                        case 2: // extractMax
//                            std::cout << "ExtractMax" << std::endl;
//                            opt_val = pqArray->extractMax();
//                            if(opt_val != std::nullopt){
//                                std::cout << "Extracted value: " << opt_val.value() << std::endl;
//                            } else std::cout << "Empty structure" << std::endl;
//                            break;
//                        case 3: // peek
//                            std::cout << "Peek" << std::endl;
//                            opt_val = pqArray->peek();
//                            if(opt_val != std::nullopt){
//                                std::cout << "Peeked value: " << opt_val.value() << std::endl;
//                            } else std::cout << "Empty structure" << std::endl;
//                            break;
//                        case 4: // modifyKey
//                            std::cout << "ModifyKey" << std::endl;
//                            std::cout << "Old priority: " ;
//                            priority = typeNumber<int>();
//                            std::cout << "Value: " ;
//                            value = typeNumber<int>();
//                            std::cout << "New priority: " ;
//                            new_priority = typeNumber<int>();
//                            value = pqArray->modifyKey(PriorityValue(priority,value), new_priority);
//                            if(value == 0) std::cout << "Success" << std::endl;
//                            else if (value == 1) std::cout << "The same priority" << std::endl;
//                            else if (value == 2) std::cout << "No such element" << std::endl;
//                            else if (value == 3) std::cout << "New priority less than zero" << std::endl;
//                            break;
//                        case 5: // display
//                            std::cout << "RerurnSize" << std::endl;
//                            std::cout << "Size: " << pqArray->getSize() << std::endl;
//                            break;
//                        case 6: // display
//                            pqArray->display();
//                            break;
//                        default:
//                            std::cout << "Wrong choice. Try again." << std::endl;
//                            break;
//                    }
//                } while (secondChoice != 0);
//                break;
//                /*  AUTO TEST   */
//            case 3:
//                std::cout << std::endl << "AUTO-TEST" << std::endl;
//                std::cout << "Repetitions: " ;
//                size = typeNumber<int>();
//                autotest(size);
//                break;
//            default:
//                std::cout << "Wrong choice. Try again." << std::endl;
//                break;
//        }
//    } while (firstChocie != 0);
//    exit(0);
//}
