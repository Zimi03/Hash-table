#pragma once

#include <cstring>
#include <iostream>
#include <optional>
//#include "PriorityValue.hpp"
//#include "PriorityValueOrder.hpp"

template <typename Data>
class DynamicArray {
protected:
    Data *array = nullptr;
    int size;
    int capacity;

public:
    DynamicArray() {
        capacity = 5;
        array = new Data[capacity];
        size = 0;
    }

    DynamicArray(int capacity_) {
        capacity = capacity_;
        array = new Data[capacity];
        size = 0;
    }

    DynamicArray(DynamicArray *to_copy): size(0), capacity(5) {
        std::optional<Data> value;
        int _size = to_copy->getSize();
        array = new Data[_size];
        if (_size != 0) {
            for (int i = 0; i < _size; i++) {
                value = to_copy->get(i);
                if (value != std::nullopt) {
                    insertBack(value.value());
                }
            }
        }
    }

    ~DynamicArray() {
        if (array != nullptr) {
            // Jeśli Data to wskaźnik do tablicy dynamicznej
            if constexpr (std::is_pointer<Data>::value && !std::is_array<Data>::value) {
                for (int i = 0; i < size; ++i) {
                    delete array[i]; // Zwolnienie pamięci dla każdego elementu tablicy
                }
            }
            delete[] array; // Zwolnienie pamięci dla tablicy
        }
    }

    int getSize() {
        return size;
    }

    bool isEmpty() {
        return size == 0;
    }

    int getCapacity() {
        return capacity;
    }

    int swap(int a, int b) {
        if(a >= 0 && a < size && b >= 0 && b < size && a != b) {
            Data tmp = array[a];
            array[a] = array[b];
            array[b] = tmp;
            return 0;
        } else return 1;
    }

    /**
     * Chnages element on given index.
     * @return 0 - succes
     * @return 1 - failure
     */
    int change(int index, Data &new_value) {
        if(index >= 0 && index < size){
            array[index] = new_value;
            return 0;
        } return 1;
    }

    int find(Data value){
        for (int i = 0; i < size; i++) {
            if (array[i] == value) {
                return i;
            }
        }
        return -1;
    }

    std::optional<Data> get(int index) {
        if(index >= 0 && index < size) {
            return array[index];
        } else return std::nullopt;
    }

    void init(Data data) {
        insertBack(data);
    }

    void insertFront(Data data) {
        if (size == capacity) {
            grow();
        }
        for (int i = size - 1; i >= 0; i--) {
            array[i + 1] = array[i];
        }

        array[0] = data;
        size ++;
    }

    int insert(int index, Data data) {
        if (index == 0) {
            insertFront(data);
            return 0;
        } else if (size == index) {
            insertBack(data);
            return 0;
        }
        if (index >= 0 && index <= size) {
            if (size == capacity) {
                grow();
            }
            for (int i = size - 1; i >= index; i--) {
                array[i + 1] = array[i];
            }

            array[index] = data;
            size++;
            return 0;
        }
        return 1;
    }

    void insertBack(Data data){
        if (size == capacity) grow();
        array[size] = data;
        size++;
    }

    std::optional<Data> removeFront() {
        if (size != 0) {
            Data data = array[0];
            for (int i = 0; i < size; i++) {
                array[i] = array[i + 1];
            }
            size--;
            if (size < (capacity/2)) shrink();
            return data;
        } else return std::nullopt;
    }

    std::optional<Data> remove(int index) {
        if (index < 0 && index >= size) return std::nullopt;
        if (index == 0) return removeFront();
        Data data = array[index];
        for (int i = index; i < size; i++) {
            array[i] = array[i + 1];
        }
        size--;
        if(size < (capacity/2)) shrink();
        return data;
    }

    std::optional<Data> removeBack() {
        if (size == 0) return std::nullopt;
        return remove(size-1);
    }

    void grow() {
        capacity *= 2; // doubling capacity of array
        Data *temp = new Data[capacity]; // creating temp array
        for( int i =0; i< size; i++){
            temp[i] = array[i]; //copying values to temp array
        }
        delete[] array; // dellocating memory of old array
        array = temp; //copying temp array to new array
    }

    void shrink() {
        capacity = size;
        Data *temp = new Data[capacity];
        for (int i = 0; i < size; i++) {
            temp[i] = array[i];
        }
        delete[] array;
        array = temp;
    }
};
