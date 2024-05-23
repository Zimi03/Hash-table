#pragma once

#include <random>
#include <algorithm>
#include <chrono>
#include <map>

namespace Utils {
    /**
     * Generuje liczby z zakresu
     * @return
     */
    int generateNumber(int max) {
        std::random_device rd;
        std::mt19937 gen(std::random_device{}());
        std::uniform_int_distribution<int> dist(0, max);
        return dist(gen);
    }

}
