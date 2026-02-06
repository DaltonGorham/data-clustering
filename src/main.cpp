/*
    Coding Standards: C++ Core Guidelines
    https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines
    C++ language standard version: C++20
*/
#include <iostream>
#include "../include/FileParser.h"
#include "../include/Utilities.h"
#include "../include/KMeans.h"

int main(int argc, char* argv[]) {
    auto config = Config::parseArgs(argc, argv);

    auto lines = FileParser::readFile(config.inputFile);
    Dataset dataset = FileParser::parseFileContents(lines, config.inputFile);
    KMeans kmeans(config.numOfClusters, config.maxIterations,
                  config.convergenceThreshold, config.numOfRuns);
    kmeans.run(dataset);
}
