/*
    Coding Standards: C++ Core Guidelines
    https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines
    C++ language standard version: C++20
*/
#include <iostream>
#include "../include/FileParser.h"
#include "../include/Utilities.h"
#include "../include/KMeans.h"
// TODO: 
/*
    - now that i have the basic setup working. we need to make this more efficient. 
    1. replce the map std::map<int, DataPoints> clusters; with a vector? less overhead with copying and lookups of maps. 
    2. probably can optimize the updateClusterCenters method as well.
    3. maybe add some multithreading for the runs and iterations. this might be adding more complexity than its worth. 
    4. maybe add more error handling
*/
int main(int argc, char* argv[]) {
    auto config = Config::parseArgs(argc, argv);

    auto lines = FileParser::readFile(config.inputFile);
    Dataset dataset = FileParser::parseFileContents(lines, config.inputFile);
    KMeans kmeans(config.numOfClusters, config.maxIterations,
                  config.convergenceThreshold, config.numOfRuns);
    kmeans.run(dataset);
}
