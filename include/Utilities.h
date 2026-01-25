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
        static bool validateInputFile(const char* filePath);
        static bool validateClusters(char* clusterArg);
        static bool validateIterations(char* iterationsArg);
        static bool validateThreshold(char* thresholdArg);
        static bool validateRuns(char* runsArg);
};
