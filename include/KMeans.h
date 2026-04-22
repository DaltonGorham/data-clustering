/*
    Coding Standards: C++ Core Guidelines
    https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines
    C++ language standard version: C++20
*/
#pragma once
#include "Dataset.h"
#include "KMeansResult.h"
#include "../include/Utilities.h"
#include <utility>
#include <vector>

enum class InitMethod {
    RandomInit,
    RandomPartition
};

class KMeans {
    private:
        Config m_config;
        InitMethod m_initMethod;
        InitPerformace m_bestInitPerformance;
        std::vector<KMeansResult> m_kResults;
        DataPoints m_clusterCenters;
        double m_bestRandIndex = 0.0;
        double m_bestJaccardIndex = 0.0;

        double getEuclideanDistance(const std::vector<double>&point, const std::vector<double>& centroid);
        void updateClusterCenters(std::vector<int>& clusterCenterIndices, const DataPoints& dataPoints, int dimensions, int numOfClusters);
        std::pair<int, double> getClosestClusterCenter(const std::vector<double>& point, const DataPoints& clusterCenters);
        DataPoints getInitialCenters(const Dataset& dataset, int numOfClusters) const;
        std::string getInitMethod() const;
        double assignPointsAndComputeSSE(const DataPoints& points, std::vector<int>& clusterCenterIndices);
        InitPerformace computeBestPerformance(const std::vector<InitPerformace>& results) const;
    public:
        KMeans(const Config& config, InitMethod initMethod)
            : m_config(config), m_initMethod(initMethod), m_bestInitPerformance{} {}
        void run(const Dataset& dataset);
        const DataPoints& getClusterCenters() const { return m_clusterCenters; }
        const InitPerformace& getPerformance() const { return m_bestInitPerformance; }
        const std::vector<KMeansResult>& getKMeansResults() const { return m_kResults; }
        double getBestRandIndex() const { return m_bestRandIndex; }
        double getBestJaccardIndex() const { return m_bestJaccardIndex; }
};