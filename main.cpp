 #include <iostream>
 #include <vector>
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
 * @param[out] zadanie  - zwracany przez parametr numer zadania wybrany przez użytkownika
 */
 void menu(unsigned int &first)
{
    std::cout << "\nMENU"  << std::endl;
    std::cout << "0. EXIT." << std::endl;
    std::cout << "1. OpenAdressing" << std::endl;
    std::cout << "2. CuckooHash" << std::endl;
    std::cout << "3. Array" << std::endl;
     std::cout << "4. Auto-test" << std::endl;


    std::cout << "Choose task: ";
    first = typeNumber<unsigned int>();
}


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

    // Losowanie danych inicjalizujących dla każdego rozmiaru
    std::cout << "Randoming initzialization data..." << std::endl;
    for (const auto &data: Benchmark::TESTING_SIZES) { // uzupełnia tablice danych inicjalizujących
        initzialData[data] = new Pair<int, int>[data]; // alokowanie odpowiedniego rozmiaru tablicy
        for (int i = 0; i < data; i++) {
            initzialData[data][i] = Pair<int, int>(data);
        }
    }

    // Mapa rozmiar struktury -> tablica danych testujących funkcje insert
    std::map<
            int, // rozmiar
            Pair<int, int> * // tablica danych
    > testingDataInsert;

    // Losowanie danych testowych funkcji insert dla każdego rozmiaru
    std::cout << "Randoming testing data..." << std::endl;
    for (const auto &data: Benchmark::TESTING_SIZES) { // uzupełnia tablice danych testujących - dla INSERT
        testingDataInsert[data] = new Pair<int, int>[data]; // alokowanie odpowiedniego rozmiaru tablicy
        for (int i = 0; i < data; i++) {
            testingDataInsert[data][i] = Pair<int, int>(data);
        }
    }

    // Mapa rozmiar struktury -> tablica danych testujących funkcje modify
    std::map<
            int, // rozmiar
            Pair<int, int> * // tablica danych
    > testingDataModify;

    // Losowanie danych testowych funkcji modify dla każdego rozmiaru
    for (const auto &data: Benchmark::TESTING_SIZES) { // uzupełnia tablice danych testujących - dla INSERT
        testingDataModify[data] = new Pair<int, int>[data]; // alokowanie odpowiedniego rozmiaru tablicy
        for (int i = 0; i < data; i++) {
            testingDataModify[data][i] = Pair<int, int>(initzialData[data], data);
        }
    }

    std::cout << "Randoming data finished." << std::endl << std::endl;

    std::cout << "Testing data..." << std::endl << std::endl;

    DataExporter exporter; // inicjalizuje exporter danych do csv poprzez nazwe testu i wyniki testów
    std::string folderPath = exporter.createNewFolder();
    for (const auto &test: tests) { // dla wszystkich testów
        std::vector<Results> results; // vektor rezultatów
        std::cout << "Method: " << test.first << "..." << std::endl;
        if (test.first == "ModifyKey") {
            results = Benchmark::run(test.second, initzialData, testingDataModify, repetitions); // funkcja testująca
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
    int size, priority, value, new_priority;
    std::optional<Pair<int,int>> opt_val;

    OpenAddressing<int, int>* OpenAddressingTable = nullptr;
    CuckooHashTable<int, int>* CuckooHashTable = nullptr;

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
                OpenAddressingTable = new OpenAddressing<int, int>; 
                for(int i = 0; i < size; i++){
                    OpenAddressingTable->insert(Utils::generateNumber(size*5),Utils::generateNumber(size));
                }

                do {
                    menuSecondLevel(firstChocie, secondChoice, size);
                    switch (secondChoice) {
                        case 0:
                            std::cout << "Quiting from PriorityQueue - heap" << std::endl;
                            delete OpenAddressingTable;
                            break;
                        case 1: // insert
                            std::cout << "Insert" << std::endl;
                            std::cout << "Priority: " ;
                            priority = typeNumber<int>();
                            std::cout << "Value: " ;
                            value = typeNumber<int>();
                            OpenAddressingTable->insert(priority,value);
                            break;
                        case 2: // remove
                            std::cout << "Remove" << std::endl;
                            std::cout << "Priority: " ;
                            priority = typeNumber<int>();
                            opt_val = OpenAddressingTable->remove(priority);
                            if(opt_val != std::nullopt){
                                std::cout << "Extracted value: " << opt_val.value() << std::endl;
                            } else std::cout << "Empty structure" << std::endl;
                            break;

                    }
                } while (secondChoice != 0);
                break;
                /*  PriorityQueue - dynamic array   */
            // case 2: // PriorityQueue - dynamic array
            //     std::cout << "Structure size: " ;
            //     size = typeNumber<int>();
            //     pqArray = new PriorityQueueArray;
            //     for(int i = 0; i < size; i++){
            //         pqArray->insert(PriorityValue(Utils::generateNumber(size*5),Utils::generateNumber(size)));
            //     }
            //     do {
            //         menuSecondLevel(firstChocie, secondChoice, size);
            //         switch (secondChoice) {
            //             case 0:
            //                 std::cout << "Quiting from PriorityQueue - dynamic array" << std::endl;
            //                 delete pqArray;
            //                 break;
            //             case 1: // insert
            //                 std::cout << "Insert" << std::endl;
            //                 std::cout << "Priority: " ;
            //                 priority = typeNumber<int>();
            //                 std::cout << "Value: " ;
            //                 value = typeNumber<int>();
            //                 pqArray->insert(PriorityValue(priority,value));
            //                 break;
            //             case 2: // extractMax
            //                 std::cout << "ExtractMax" << std::endl;
            //                 opt_val = pqArray->extractMax();
            //                 if(opt_val != std::nullopt){
            //                     std::cout << "Extracted value: " << opt_val.value() << std::endl;
            //                 } else std::cout << "Empty structure" << std::endl;
            //                 break;
            //             case 3: // peek
            //                 std::cout << "Peek" << std::endl;
            //                 opt_val = pqArray->peek();
            //                 if(opt_val != std::nullopt){
            //                     std::cout << "Peeked value: " << opt_val.value() << std::endl;
            //                 } else std::cout << "Empty structure" << std::endl;
            //                 break;
            //             case 4: // modifyKey
            //                 std::cout << "ModifyKey" << std::endl;
            //                 std::cout << "Old priority: " ;
            //                 priority = typeNumber<int>();
            //                 std::cout << "Value: " ;
            //                 value = typeNumber<int>();
            //                 std::cout << "New priority: " ;
            //                 new_priority = typeNumber<int>();
            //                 value = pqArray->modifyKey(PriorityValue(priority,value), new_priority);
            //                 if(value == 0) std::cout << "Success" << std::endl;
            //                 else if (value == 1) std::cout << "The same priority" << std::endl;
            //                 else if (value == 2) std::cout << "No such element" << std::endl;
            //                 else if (value == 3) std::cout << "New priority less than zero" << std::endl;
            //                 break;
            //             case 5: // display
            //                 std::cout << "RerurnSize" << std::endl;
            //                 std::cout << "Size: " << pqArray->getSize() << std::endl;
            //                 break;
            //             case 6: // display
            //                 pqArray->display();
            //                 break;
            //             default:
            //                 std::cout << "Wrong choice. Try again." << std::endl;
            //                 break;
            //         }
            //     } while (secondChoice != 0);
            //     break;
                /*  AUTO TEST   */
            case 3:
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
