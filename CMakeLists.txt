cmake_minimum_required(VERSION 3.27)
project(Hash_table)

set(CMAKE_CXX_STANDARD 20)

include_directories(HashTables)
include_directories(Structures)
include_directories(Tools)

add_executable(Hash_table
        HashTables/CuckooHashTable.cpp
        HashTables/CuckooHashTable.h
        HashTables/HashTableArray.cpp
        HashTables/HashTableArray.h
        HashTables/IHashTable.h
        Structures/DynamicArray.hpp
        Structures/Pair.hpp
        Tools/Benchmarks.hpp
        Tools/DataExporter.hpp
        Tools/Results.hpp
        Tools/Utils.hpp
        main.cpp
        HashTables/OpenAddressing.hpp)
