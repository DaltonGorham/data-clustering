/*
    Coding Standards: C++ Core Guidelines
    https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines
    C++ language standard version: C++20
*/
#include "../include/KMeans.h"
#include "../include/Utilities.h"
#include <iostream>
#include <stdexcept>
#include <vector>
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
void KMeans::updateClusterCenters(std::vector<int>& clusterCenterIndices, const DataPoints& dataPoints, int dimensions, int numOfClusters) {

    DataPoints newCenters(numOfClusters, std::vector<double>(dimensions, 0.0));
    std::vector<int> counts(numOfClusters, 0);
    
    for (int pointIndex = 0; pointIndex < clusterCenterIndices.size(); pointIndex++) {
        int cluster = clusterCenterIndices[pointIndex];
        counts[cluster]++;
        for (int d = 0; d < dimensions; d++) {
            newCenters[cluster][d] += dataPoints[pointIndex][d];
        }
    }

    for (int cluster = 0; cluster < numOfClusters; cluster++) {
        if (counts[cluster] == 0) continue;
        for (int d = 0; d < dimensions; d++) {
            newCenters[cluster][d] /= counts[cluster];
        }
        m_clusterCenters[cluster] = newCenters[cluster];
    }
}

double KMeans::assignPointsAndComputeSSE(const DataPoints& points, std::vector<int>& clusterCenterIndices) {
    double sse = 0.0;
    for (int i = 0; i < points.size(); i++) {
        auto [cluster, distance] = getClosestClusterCenter(points[i], m_clusterCenters);
        clusterCenterIndices[i] = cluster;
        sse += distance;
    }
    return sse;
}

InitPerformace KMeans::computeBestPerformance(const std::vector<InitPerformace>& results) const {
    InitPerformace best = results[0];
    for (const auto& result : results) {
        if (result.initialSSE < best.initialSSE) {
            best.initialSSE = result.initialSSE;
        }
        if (result.finalSSE < best.finalSSE) {
            best.finalSSE = result.finalSSE;
        }
        if (result.numOfIterations < best.numOfIterations) {
            best.numOfIterations = result.numOfIterations;
        }
    }
    return best;
}

DataPoints KMeans::getInitialCenters(const Dataset& dataset, int numOfClusters) const {
    switch (m_initMethod) {
        case InitMethod::RandomInit:      
            return dataset.getRandomClusterCenters(numOfClusters);
        case InitMethod::RandomPartition: 
            return dataset.getRandomPartitionCenters(numOfClusters);
        default:
            std::cerr << "Error: Invalid initialization method.\n";
            std::exit(EXIT_FAILURE);
    }
}

std::string KMeans::getInitMethod() const {
    switch (m_initMethod) {
        case InitMethod::RandomInit:      
            return "random_init";
        case InitMethod::RandomPartition: 
            return "random_partition";
        default:
            std::cerr << "Error: Invalid initialization method.\n";
            std::exit(EXIT_FAILURE);
    }
}

void KMeans::run(const Dataset& dataset) {
    int maxClusters = dataset.getKMax();
    int dimensions = dataset.getDimensions();
    const auto& points = dataset.getDataPoints();
    std::vector<int> clusterCenterIndices(points.size());

    for (int numOfClusters = Config::MIN_CLUSTERS; numOfClusters <= maxClusters; numOfClusters++) { 

        m_clusterCenters = getInitialCenters(dataset, numOfClusters);
        KMeansResult kMeansResult;

        for (int run = 0; run < m_config.numOfRuns; run++) {

            if (run > 0) m_clusterCenters = getInitialCenters(dataset, numOfClusters);

            int iteration = 0;
            bool converged = false;
            double sse = 0.0;

            while (iteration < m_config.maxIterations && !converged) {
                double newSSE = assignPointsAndComputeSSE(points, clusterCenterIndices);
                updateClusterCenters(clusterCenterIndices, points, dimensions, numOfClusters);

                if (iteration > 0) {
                    converged = (sse - newSSE) / sse < m_config.convergenceThreshold;
                }
                sse = newSSE;
                iteration++;
            }

            if (sse < kMeansResult.bestSSE || run == 0) {
                kMeansResult.bestSSE = sse;
                kMeansResult.bestCenters = m_clusterCenters;
                kMeansResult.bestClusterAssignments = clusterCenterIndices;
            }

        }

        kMeansResult.numOfClusters = numOfClusters;
        kMeansResult.calinskiHarabaszIndex = dataset.calinskiHarabaszIndex(
            kMeansResult.bestCenters, kMeansResult.bestClusterAssignments, kMeansResult.bestSSE);
        kMeansResult.silhouetteWidth = dataset.silhouetteWidth(
            kMeansResult.bestCenters, kMeansResult.bestClusterAssignments);
        m_kResults.push_back(kMeansResult);
    }
}