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
#include <map>

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

    /*
        going through this algorithm currently for testing purposes
        1. right now we have implemented assigning each point to the closest cluster center
        2. next we need to compute the new cluster centers based on the assigned points
            something like newClusterCenters = the average of all points assigned to that cluster
        3. then we need to calculate SSE and check for convergence

        map clusterCenter index to data points assigned to that cluster
        then we can easily compute new cluster centers by averaging the points in each cluster
    */
    KMeans kmeans(numOfClusters, maxIterations, convergenceThreshold, clusterCenters);
    std::map<int, DataPoints> clusters;

    // LOTS OF DEBUGGING OUTPUT HERE
    // TODO: move all kmeans logic to KMeans.cpp. maybe a run() method that executes the full algorithm
    for (const auto& point : dataset.getDataPoints()) {
        int closestCluster = kmeans.getClosestClusterCenter(point, clusterCenters);
        clusters[closestCluster].push_back(point);
    }

    std::cout << "Initial Cluster Centers" << "\n";
    for (const auto& center : clusterCenters) {
        for (const auto& value : center) {
            std::cout << value << " ";
        }
        std::cout << "\n";
    }
    std::cout << "======================" << "\n";

    for (const auto& [clusterIndex, points] : clusters) {
        std::cout << "Cluster " << clusterIndex << ":\n";
        std::cout << "Num of Points: " << points.size() << "\n";
        for (int i = 0; i < points.size(); i++) {
            for (const auto& value : points[i]) {
                std::cout << value << " ";
            }
            std::cout << "\n";
        }
        std:: cout << "======================\n";
    }
    
}
