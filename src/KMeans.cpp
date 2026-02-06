/*
    Coding Standards: C++ Core Guidelines
    https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines
    C++ language standard version: C++20
*/
#include "../include/KMeans.h"
#include "../include/Utilities.h"
#include <iostream>
#include <stdexcept>
#include <map>
#include <cmath>

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

void KMeans::updateClusterCenters(std::map<int, DataPoints>& clusters, int dimensions) {

    if (clusters.empty()) {
        std::cerr << "Error: No clusters to update." << "\n";
        std::cerr << "Cannot update cluster centers." << "\n";
        exit(EXIT_FAILURE);
    }

    for (const auto& [clusterIndex, points] : clusters) {
        std::vector<double> newCenter(dimensions, 0.0);
        for (const auto& point : points) {
            for (int i = 0; i < dimensions; i++) {
                newCenter[i] += point[i];
            }
        }
        for (int i = 0; i < dimensions; i++) {
            newCenter[i] /= points.size();
        }
        m_clusterCenters[clusterIndex] = newCenter;
    }
}


void KMeans::run(const Dataset& dataset) {
    m_clusterCenters = dataset.getRandomClusterCenters(m_numOfClusters);
    int dimensions = dataset.getDimensions();
    std::map<int, DataPoints> clusters;
    std::map<int, double> bestRunMap;
    std::string output = "";
    double sse = 0.0;

   for (int run = 0; run < m_numOfRuns; run++) {
    
        if (run > 0) {
            m_clusterCenters = dataset.getRandomClusterCenters(m_numOfClusters);
        }
        
        std::string runString = std::to_string(run + 1);
        output += "Run " + runString + "\n" + std::string(4 + runString.length(), '-') + "\n";

        int iteration = 0;
        bool converged = false;
        sse = 0.0;  
        
        while (iteration < m_maxIterations && !converged) {
            clusters.clear();
            
            for (const auto& point : dataset.getDataPoints()) {
                int closestCluster = getClosestClusterCenter(point, m_clusterCenters);
                clusters[closestCluster].push_back(point);
            }
            
            updateClusterCenters(clusters, dimensions);
            
            double newSSE = 0.0;
            for (const auto& [clusterIndex, points] : clusters) {
                for (const auto& point : points) {
                    newSSE += getEuclideanDistance(point, m_clusterCenters[clusterIndex]);
                }
            }

            output += "Iteration " + std::to_string(iteration + 1) + ": SSE = " + std::to_string(newSSE) + "\n";
            
            if (iteration > 0) {
                converged = std::abs(sse - newSSE) < m_convergenceThreshold;
            }
            sse = newSSE;
            iteration++;
        }
        output += "\n";
        bestRunMap[run] = sse;
    }

    double bestSSE = bestRunMap.begin()->second;
    int bestRun = 0;
    for (const auto& [run, runSSE] : bestRunMap) {
        if (runSSE < bestSSE) {
            bestSSE = runSSE;
            bestRun = run;
        }
    }
    
    m_SSE = bestSSE;
    output += "Best Run: " + std::to_string(bestRun + 1) + ", SSE = " + std::to_string(bestSSE) + "\n";
    Utilities::writeToFile(dataset.getInputFile(), output);
}