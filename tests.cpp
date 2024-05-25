/*
#include "HashTables/OpenAddressing.hpp"
#include "Tools/Utils.hpp"
#include "HashTables/CuckooHashTable.h"
#include "HashTables/HashTableArray.h"

int main()
{

    //    CuckooHashTable<int, int> hash_table;
    OpenAddressing<int, int> hash_table;
    int size = 20;
    Pair<int, int> *para;
    std::cout << "\nSTART\n";
    Pair<int, int> array[size];


    for (int i = 0; i < size; i++)
    {
        std::cout << i << "\n";
        array[i] = Pair<int, int>(Utils::generateNumber(100), i);
        hash_table.insert(array[i].key, array[i].value);
    }
    OpenAddressing<int, int> hash_table2(hash_table);
    //    CuckooHashTable<int, int> hash_table2(hash_table);

    hash_table2.display();
    for (int i = 0; i < size; i++)
    {
        std::cout << "REMOVE " << array[i];
        int index = hash_table2.remove(array[i].key);
        if (index == 0)
            std::cout << " - Usunięto " << std::endl;
        //        if(index != -1) std::cout << " - Usunięto - indeks: " << index << std::endl;
        else
            std::cout << " - NIE usunięto" << std::endl;
    }

    hash_table2.display();
    hash_table.display();
}
*/

