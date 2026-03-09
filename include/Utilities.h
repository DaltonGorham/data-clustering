/*
    Coding Standards: C++ Core Guidelines
    https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines
    C++ language standard version: C++20
*/
#pragma once
#include <string>
#include <vector>

const int EXPECTED_ARGS = 6;

enum ArgumentPositions {
    INPUT_FILE = 1,
    NUMBER_OF_CLUSTERS,
    MAX_ITERATIONS,
    CONVERGENCE_THRESHOLD,
    NUMBER_OF_RUNS
};

struct Config {
    std::string inputFile;
    int numOfClusters;
    int maxIterations;
    double convergenceThreshold;
    int numOfRuns;

    static Config parseArgs(int argc, char* argv[]);
};

struct InitPerformace {
    std::string initMethod;
    double initialSSE;
    double finalSSE;
    int numOfIterations;
};

class Utilities {
    public:
        static void validateArgs(int argc, char* argv[], int expectedArgs);
        static void writeToFile(const std::string& inputFile, const std::string& lines, const std::string& suffix);
        static void writeToCSV(const std::string& inputFile, const std::vector<InitPerformace>& results);
        static std::string doubleToStr(double value);
    private:
        static void validateNumberOfArgs(int argc, char* argv[], int expectedArgs);
        static void validateArgTypes(char* argv[]);
        static bool validateInputFile(const char* filePath);
        static bool validateClusters(char* clusterArg);
        static bool validateIterations(char* iterationsArg);
        static bool validateThreshold(char* thresholdArg);
        static bool validateRuns(char* runsArg);
};
