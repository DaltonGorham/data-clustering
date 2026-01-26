/*
    Coding Standards: C++ Core Guidelines
    https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines
    C++ language standard version: C++20
*/
#include "../include/Utilities.h"
#include <iostream>
#include <cstdlib>
#include <string>
#include <filesystem>

void Utilities::validateArgs(int argc, char* argv[], int expectedArgs) {
    validateNumberOfArgs(argc, argv, expectedArgs);
    validateArgTypes(argv);
}

void Utilities::validateNumberOfArgs(int argc, char* argv[], int expectedArgs) {
    if (argc != expectedArgs) {
        std::cerr << "Usage: " << argv[0]
            << " <input_file>"
            << " <number_of_clusters>"
            << " <max_iterations>"
            << " <convergence_threshold>"
            << " <number_of_runs>\n";
        std::exit(EXIT_FAILURE);
    }
}

void Utilities::validateArgTypes(char* argv[]) {
    if (!validateInputFile(argv[INPUT_FILE])) {
        std::cerr << "Error: Input file: " << "`" << argv[INPUT_FILE] << "` does not exist relative to the executable.\n";
        std::cerr << "Error: Please provide a valid file path or place the file in the correct directory.\n";
        std::exit(EXIT_FAILURE);
    }
    if (!validateClusters(argv[NUMBER_OF_CLUSTERS])) {
        std::cerr << "number_of_clusters: " << "`" << argv[NUMBER_OF_CLUSTERS] << "` must be an integer greater than 1.\n";
        std::exit(EXIT_FAILURE);
    }
    if (!validateIterations(argv[MAX_ITERATIONS])) {
        std::cerr << "max_iterations: " << "`" << argv[MAX_ITERATIONS] << "` must be a positive integer.\n";
        std::exit(EXIT_FAILURE);
    }
    if (!validateThreshold(argv[CONVERGENCE_THRESHOLD])) {
        std::cerr << "convergence_threshold: " << "`" << argv[CONVERGENCE_THRESHOLD] << "` must be a positive real number.\n";
        std::exit(EXIT_FAILURE);
    }
    if (!validateRuns(argv[NUMBER_OF_RUNS])) {
        std::cerr << "number_of_runs: " << "`" << argv[NUMBER_OF_RUNS] << "` must be a positive integer.\n";
        std::exit(EXIT_FAILURE);
    }
}

bool Utilities::validateInputFile(const char* filePath) {
    return std::filesystem::exists(filePath);
}

bool Utilities::validateClusters(char* clusterArg) {
    std::string argString = clusterArg;
    for (const auto& c : argString) {
        if (!std::isdigit(c)) return false;
    }
    return std::stoi(argString) > 1;
}

bool Utilities::validateIterations(char* iterationsArg) {
    std::string argString = iterationsArg;
    for (const auto& c : argString) {
        if (!std::isdigit(c)) return false;
    }
    return std::stoi(argString) > 0;
}

bool Utilities::validateThreshold(char* thresholdArg) {
    std::string argString = thresholdArg;
    size_t pos;
    double threshold;
    try {
        threshold = std::stod(argString, &pos);
        if (pos != argString.length()) return false;
    } catch (...) {
        return false;
    }
    return threshold > 0.0;
}

bool Utilities::validateRuns(char* runsArg) {
    std::string argString = runsArg;
    for (const auto& c : argString) {
        if (!std::isdigit(c)) return false;
    }
    return std::stoi(argString) > 0;
}
