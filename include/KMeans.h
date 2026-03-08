/*
    Coding Standards: C++ Core Guidelines
    https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines
    C++ language standard version: C++20
*/
#pragma once
#include "Dataset.h"
#include "../include/Utilities.h"
#include <map>
#include <utility>

class KMeans {
    private:
        Config m_config;
        double m_SSE;
        DataPoints m_clusterCenters;

        double getEuclideanDistance(const std::vector<double>&point, const std::vector<double>& centroid);
        void updateClusterCenters(std::vector<int>& clusterCenterIndices, const DataPoints& dataPoints, int dimensions);
        std::pair<int, double> getClosestClusterCenter(const std::vector<double>& point, const DataPoints& clusterCenters);
    public:
        KMeans(const Config& config)
            : m_config(config), m_SSE(0.0) {}
        void run(const Dataset& dataset);
        double getSSE() const { return m_SSE; }
        const DataPoints& getClusterCenters() const { return m_clusterCenters; }
};