/*
    Coding Standards: C++ Core Guidelines
    https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines
    C++ language standard version: C++20
*/
#pragma once
#include "Dataset.h"

class KMeans {
    private:
        int m_numOfClusters;
        int m_maxIterations;
        double m_convergenceThreshold;
        double m_SSE;
        DataPoints m_clusterCenters;

        double getEuclideanDistance(const std::vector<double>&point, const std::vector<double>& centroid);
    public:
        KMeans(int numOfClusters, int maxIterations, double convergenceThreshold, DataPoints initialCenters)
            : m_numOfClusters(numOfClusters), m_maxIterations(maxIterations), m_convergenceThreshold(convergenceThreshold),
              m_clusterCenters(initialCenters) {}
            int getClosestClusterCenter(const std::vector<double>& point, const DataPoints& clusterCenters);
            const double getSSE() const { return m_SSE; }
            const DataPoints& getClusterCenters() const { return m_clusterCenters; }
};