#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <filesystem>
#include "Results.hpp"

/**
 * Obiekt przechowujący dane do zapisu do pliku
 * Ma metodę zapisującą dane do pliku
 */
class DataExporter {
    std::string name;
    std::vector<Results> results;

    public:
        DataExporter(
            const std::string _name,
            const std::vector<Results>& _results
        ) :  name(_name), results(_results) {}

        DataExporter() : name(" ") {}

        /**
         * Zapis danych pomiarowych do pliku csv
         */
        void generate(std::string folder_path) {

            std::fstream fs(filePath(folder_path, filename()), std::ios::out);

            if (!fs.is_open()) {
                std::cerr << "Could not open file " << filename() << std::endl;
                return;
            }

            const std::string header = "className;stack;average;repetitions\n";
            fs << header;

            for (auto result : results) {
                fs << result.className << ";" << result.stack << ";" << result.average() << ";" << result.testsNumber() << "\n";
            }

            fs.close();
        }

        std::string createNewFolder(){
            std::filesystem::path baseDir = std::filesystem::current_path().parent_path() / "Results";

            // Zliczanie istniejących katalogów Results
            int maxResults = 0;
            for (const auto& entry : std::filesystem::directory_iterator(baseDir)) {
                if (entry.is_directory()) {
                    std::string dirname = entry.path().filename().string();
                    if (dirname.find("Results") == 0) {
                        std::string numberStr = dirname.substr(7); // Pomiń "Results"
                        int number = std::stoi(numberStr);
                        maxResults = std::max(maxResults, number);
                    }
                }
            }

            // Określenie nazwy nowego katalogu
            int nextResultNumber = maxResults + 1;
            std::string newDirName = "Results" + std::to_string(nextResultNumber);
            std::filesystem::path newDir = baseDir / newDirName;

            // Tworzenie nowego katalogu
            try {
                if (!std::filesystem::exists(newDir)) {
                    if (std::filesystem::create_directory(newDir)) {
                        std::cout << "Utworzono katalog: " << newDir << std::endl;
                    } else {
                        std::cerr << "Nie udało się utworzyć katalogu." << std::endl;
                    }
                } else {
                    std::cerr << "Katalog już istnieje: " << newDir << std::endl;
                }
            } catch (const std::filesystem::filesystem_error& e) {
                std::cerr << "Błąd systemu plików: " << e.what() << std::endl;
                exit(0);
            }
            return newDir.string();
        }

    private:
        std::string filename() {
            return "results_" + name + ".csv";
        }
        std::string filePath(std::string _filepath, std::string _filename){
            std::filesystem::path dir = _filepath;
            return (dir / _filename).string();
        }
};
