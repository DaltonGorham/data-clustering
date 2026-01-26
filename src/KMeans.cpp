/*
    Coding Standards: C++ Core Guidelines
    https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines
    C++ language standard version: C++20
*/
#include "../include/KMeans.h"
#include <iostream>
#include <stdexcept>

double KMeans::getEuclideanDistance(const std::vector<double>& point, const std::vector<double>& centroid) {
    double distance = 0.0;
    for (int i = 0; i < point.size(); i++ ) {
        distance += (point[i] - centroid[i]) * (point[i] - centroid[i]);
    }
    return distance;
}

int KMeans::getClosestClusterCenter(const std::vector<double>& point, const DataPoints& clusterCenters) {
    if (clusterCenters.empty()) {
        std::cerr << "Error: Cluster centers are empty." << "\n";
        std::cerr << "Cannot determine the closest cluster center." << "\n";
        exit(EXIT_FAILURE);
    }

    int closestIndex = 0;
    double minDistance = getEuclideanDistance(point, clusterCenters[0]);

    for (int i = 1; i < clusterCenters.size(); i++) {
        double distance = getEuclideanDistance(point, clusterCenters[i]);
        if (distance < minDistance) {
            minDistance = distance;
            closestIndex = i;
        }
    }
    return closestIndex;
}