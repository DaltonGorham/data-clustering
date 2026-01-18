/*
    Coding Standards: C++ Core Guidelines
    https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines
    C++ language standard version: C++20
*/
#include <iostream>
#include <string>
#include "../include/FileParser.h"
#include "../include/Utilities.h"

const int ARGS = 6;

int main(int argc, char* argv[]) {
    Utilities::validateArgs(argc, argv, ARGS);
    auto lines = FileParser::readFile(argv[INPUT_FILE]);
    Dataset dataset = FileParser::parseFileContents(lines);
    std::vector<std::vector<double>> clusterCenters = dataset.getRandomClusterCenters(std::stoi(argv[NUMBER_OF_CLUSTERS]));
    dataset.outputClusterCenters(clusterCenters, argv[INPUT_FILE]);
}
