/*
    Coding Standards: C++ Core Guidelines
    https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines
    C++ language standard version: C++20
*/
#include <iostream>
#include <string>
#include "../include/FileParser.h"
#include "../include/Utilities.h"
#include "../include/KMeans.h"

const int ARGS = 6;

int main(int argc, char* argv[]) {
    Utilities::validateArgs(argc, argv, ARGS);

    const std::string inputFile = argv[INPUT_FILE];
    const int numOfClusters = std::stoi(argv[NUMBER_OF_CLUSTERS]);
    const int maxIterations = std::stoi(argv[MAX_ITERATIONS]);
    const double convergenceThreshold = std::stod(argv[CONVERGENCE_THRESHOLD]);
    const int numOfRuns = std::stoi(argv[NUMBER_OF_RUNS]);

    auto lines = FileParser::readFile(inputFile);
    Dataset dataset = FileParser::parseFileContents(lines);
    DataPoints clusterCenters = dataset.getRandomClusterCenters(numOfClusters);
    // dataset.outputClusterCenters(clusterCenters, inputFile);
    KMeans kmeans(numOfClusters, maxIterations, convergenceThreshold, clusterCenters);
    kmeans.run(dataset);
}
