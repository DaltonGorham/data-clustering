/*
    Coding Standards: C++ Core Guidelines
    https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines
    C++ language standard version: C++20
*/
#include "../include/KMeans.h"
#include <iostream>
#include <stdexcept>
#include <map>

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

void KMeans::run(const Dataset& dataset) {
    std::map<int, DataPoints> clusters;
    // dataset.outputClusterCenters(clusterCenters, inputFile);

    /*
        going through this algorithm currently for testing purposes
        1. right now we have implemented assigning each point to the closest cluster center
        2. next we need to compute the new cluster centers based on the assigned points
            something like newClusterCenters = the average of all points assigned to that cluster
        3. then we need to calculate SSE and check for convergence

        map clusterCenter index to data points assigned to that cluster
        then we can easily compute new cluster centers by averaging the points in each cluster

        m_maxIterations for loop limit
        m_convergenceThreshold to check when to stop
        m_SSE to track the sum of squared errors
    */
    
    for (const auto& point : dataset.getDataPoints()) {
        int closestCluster = getClosestClusterCenter(point, m_clusterCenters);
        clusters[closestCluster].push_back(point);
    }

    std::cout << "Initial Cluster Centers" << "\n";
    for (const auto& center : m_clusterCenters) {
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
        std::cout << "======================\n";
    }
}