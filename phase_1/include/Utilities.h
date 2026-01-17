#pragma once
#include <iostream>
#include <cstdlib>
#include <string>

 enum ArgumentPositions {
    INPUT_FILE = 1,
    NUMBER_OF_CLUSTERS,
    MAX_ITERATIONS,
    CONVERGENCE_THRESHOLD,
    NUMBER_OF_RUNS
};

class Utilities {
    public:
        static void validateArgs(int argc, char* argv[], int expectedArgs) {
            validateNumberOfArgs(argc, argv, expectedArgs);
            validateArgTypes(argv);
        }
    private:
        static void validateNumberOfArgs(int argc, char* argv[], int expectedArgs) {
            if (argc != expectedArgs) {
                std::cerr << "Usage: " << argv[0]
                    << " <input_file>"
                    << " <number_of_clusters>"
                    << " <max_iterations>"
                    << " <convergence_threshold>"
                    << " <number_of_runs>\n";
                std::exit(1);
            }
        }
        static void validateArgTypes(char* argv[]) {
            if (!validateClusters(argv[NUMBER_OF_CLUSTERS])) {
                std::cerr << "number_of_clusters must be an integer greater than 1.\n";
                std::exit(1);
            }
            if (!validateIterations(argv[MAX_ITERATIONS])) {
                std::cerr << "max_iterations must be a positive integer.\n";
                std::exit(1);
            }
            if (!validateThreshold(argv[CONVERGENCE_THRESHOLD])) {
                std::cerr << "convergence_threshold must be a positive real number.\n";
                std::exit(1);
            }
            if (!validateRuns(argv[NUMBER_OF_RUNS])) {
                std::cerr << "number_of_runs must be a positive integer.\n";
                std::exit(1);
            }
        }
        static bool validateClusters(char* arg) {
            std::string str = arg;
            for (const auto& c : str) {
                if (!std::isdigit(c)) return false;
            }
            return std::stoi(str) > 1;
        }
        static bool validateIterations(char* arg) {
            std::string str = arg;
            for (const auto& c : str) {
                if (!std::isdigit(c)) return false;
            }
            return std::stoi(str) > 0;
        }
        static bool validateThreshold(char* arg) {
            std::string str = arg;
            size_t pos;
            float threshold;
            try {
                threshold = std::stof(str, &pos);
                if (pos != str.length()) return false;
            } catch (...) {
                return false;
            }
            return threshold > 0.0f;
        }
        static bool validateRuns(char* arg) {
            std::string str = arg;
            for (const auto& c : str) {
                if (!std::isdigit(c)) return false;
            }
            return std::stoi(str) > 0;
        }
};
