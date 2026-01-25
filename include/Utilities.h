#pragma once

enum ArgumentPositions {
    INPUT_FILE = 1,
    NUMBER_OF_CLUSTERS,
    MAX_ITERATIONS,
    CONVERGENCE_THRESHOLD,
    NUMBER_OF_RUNS
};

class Utilities {
    public:
        static void validateArgs(int argc, char* argv[], int expectedArgs);
    private:
        static void validateNumberOfArgs(int argc, char* argv[], int expectedArgs);
        static void validateArgTypes(char* argv[]);
        static bool validateInputFile(const char* arg);
        static bool validateClusters(char* arg);
        static bool validateIterations(char* arg);
        static bool validateThreshold(char* arg);
        static bool validateRuns(char* arg);
};
