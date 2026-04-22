/*
    Coding Standards: C++ Core Guidelines
    https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines
    C++ language standard version: C++20
*/
#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <cmath>

using DataPoints = std::vector<std::vector<double>>;

class Dataset {
    private:
        std::string m_inputFile;
        int m_numOfPoints;
        int m_dimensions;
        int m_trueNumOfClusters;
        std::vector<int> m_trueClusterAssignments;
        DataPoints m_dataPoints;

        std::set<int> selectRandomIndices(int numOfClusters) const;
    public:
        Dataset(int numOfPoints, int dimensions, int trueNumOfClusters, const std::vector<int>& trueClusterAssignments, const DataPoints& dataPoints, const std::string& inputFile = "")
            : m_inputFile(inputFile), m_numOfPoints(numOfPoints), m_dimensions(dimensions),
              m_trueNumOfClusters(trueNumOfClusters), m_trueClusterAssignments(trueClusterAssignments),
              m_dataPoints(dataPoints) {}
        void normalize();
        void printDataset() const;
        int getNumOfPoints() const { return m_numOfPoints; }
        int getKMax() const { return static_cast<int>(std::round(std::sqrt(m_numOfPoints / 2.0))); }
        int getDimensions() const { return m_dimensions; }
        int getTrueNumOfClusters() const { return m_trueNumOfClusters; }
        std::vector<int> getTrueClusterAssignments() const { return m_trueClusterAssignments; }
        const DataPoints& getDataPoints() const { return m_dataPoints; }
        const std::string& getInputFile() const { return m_inputFile; }
        DataPoints getRandomClusterCenters(int numOfClusters) const;
        DataPoints getRandomPartitionCenters(int numOfClusters) const;
        double calinskiHarabaszIndex(const DataPoints& clusterCenters, const std::vector<int>& clusterAssignments, double sse) const;
        double silhouetteWidth(const DataPoints& clusterCenters, const std::vector<int>& clusterAssignments) const;
        double randIndex(const std::vector<int>& clusterAssignments) const;
        double jaccardIndex(const std::vector<int>& clusterAssignments) const;
};