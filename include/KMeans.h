/*
    Coding Standards: C++ Core Guidelines
    https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines
    C++ language standard version: C++20
*/
#pragma once
#include "Dataset.h"
#include <map>

class KMeans {
    private:
        int m_numOfClusters;
        int m_maxIterations;
        double m_convergenceThreshold;
        int m_numOfRuns;
        double m_SSE;
        DataPoints m_clusterCenters;

        double getEuclideanDistance(const std::vector<double>&point, const std::vector<double>& centroid);
        void updateClusterCenters(std::map<int, DataPoints>& clusterMap, int dimensions);
        int getClosestClusterCenter(const std::vector<double>& point, const DataPoints& clusterCenters);
    public:
        KMeans(int numOfClusters, int maxIterations, double convergenceThreshold, int numOfRuns)
            : m_numOfClusters(numOfClusters), m_maxIterations(maxIterations), m_convergenceThreshold(convergenceThreshold),
              m_numOfRuns(numOfRuns), m_SSE(0.0) {}
        void run(const Dataset& dataset);
        double getSSE() const { return m_SSE; }
        const DataPoints& getClusterCenters() const { return m_clusterCenters; }
};