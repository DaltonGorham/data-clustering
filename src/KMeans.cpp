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

/*
    d = sqrt( (x1-y1)² + (x2-y2)² + ... + (xn-yn)²)
    intentionally not using sqrt(), the algorithm may not converge.
*/
double KMeans::getEuclideanDistance(const std::vector<double>& point, const std::vector<double>& centroid) {
    double distance = 0.0;
    for (int i = 0; i < point.size(); i++ ) {
        distance += (point[i] - centroid[i]) * (point[i] - centroid[i]);
    }
    return distance;
}


std::pair<int, double> KMeans::getClosestClusterCenter(const std::vector<double>& point, const DataPoints& clusterCenters) {
    int closestIndex = 0;
    double minDistance = getEuclideanDistance(point, clusterCenters[0]);

    for (int i = 1; i < clusterCenters.size(); i++) {
        double distance = getEuclideanDistance(point, clusterCenters[i]);
        if (distance < minDistance) {
            minDistance = distance;
            closestIndex = i;
        }
    }
    return {closestIndex, minDistance};
}


/*
    Updates cluster centers by computing the mean of all data points assigned to each cluster.

    clusterCenterIndices - cluster assignment per point (e.g., clusterCenterIndices[0] = 2 means point 0 belongs to cluster 2)
    dataPoints           - the original dataset points
    dimensions           - number of dimensions in the dataset
*/
void KMeans::updateClusterCenters(std::vector<int>& clusterCenterIndices, const DataPoints& dataPoints, int dimensions) {

    DataPoints newCenters(m_numOfClusters, std::vector<double>(dimensions, 0.0));
    std::vector<int> counts(m_numOfClusters, 0);
    
    for (int pointIndex = 0; pointIndex < clusterCenterIndices.size(); pointIndex++) {
        int cluster = clusterCenterIndices[pointIndex];
        counts[cluster]++;
        for (int d = 0; d < dimensions; d++) {
            newCenters[cluster][d] += dataPoints[pointIndex][d];
        }
    }

    for (int cluster = 0; cluster < m_numOfClusters; cluster++) {
        for (int d = 0; d < dimensions; d++) {
            newCenters[cluster][d] /= counts[cluster];
        }
        m_clusterCenters[cluster] = newCenters[cluster];
    }
}

void KMeans::run(const Dataset& dataset) {
    m_clusterCenters = dataset.getRandomClusterCenters(m_numOfClusters);
    int dimensions = dataset.getDimensions();
    const auto& points = dataset.getDataPoints();
    std::vector<int> clusterCenterIndices(points.size());
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
            double newSSE = 0.0;

            for (int point = 0; point < points.size(); point++) {
                auto [cluster, distance] = getClosestClusterCenter(points[point], m_clusterCenters);
                clusterCenterIndices[point] = cluster;
                newSSE += distance;
            }
            
            updateClusterCenters(clusterCenterIndices, points, dimensions);
            
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