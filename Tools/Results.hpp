#pragma once

#include <vector>
#include <string>
#include <numeric>

/**
 * Klasa przechowująca rezultaty funkcji testujących
 */
class Results
{
    public:
        std::string className; // nazwa struktury
        std::vector<int> durations; // wektor pomiarów
        int stack; // rozmiar struktury testowanej


        Results(
            std::string className,
            int stack
        ) : className(className), stack(stack) {}

        /**
         * Dodaje czas pojedyńczego testu
         * @param duration
         */
        void push(int duration) {
            durations.push_back(duration);
        }

        /**
         * Zwraca średnią z pomiarów
         * @return
         */
        float average() {
            if (durations.empty()) return 0;
            auto const count = durations.size();
            return std::reduce(durations.begin(), durations.end()) / count; // suma czasów przez liczbę pomiarów
        }

        int testsNumber(){
            return int(durations.size());
        }
};
