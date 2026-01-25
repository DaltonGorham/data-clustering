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

using DataPoints = std::vector<std::vector<double>>;

class Dataset {
    private:
        int m_numOfPoints;
        int m_dimensions;
        DataPoints m_dataPoints;
        DataPoints m_clusterCenters;

        void setRandomClusterCenters(int numOfClusters);
        std::set<int> selectRandomIndices(int numOfClusters);
    public:
        Dataset(int numOfPoints, int dimensions, const DataPoints& dataPoints)
            : m_numOfPoints(numOfPoints), m_dimensions(dimensions), m_dataPoints(dataPoints) {}
        void printDataset() const;
        void outputClusterCenters(DataPoints clusterCenters, const std::string& inputFile) const;
        int getNumOfPoints() const { return m_numOfPoints; }
        int getDimensions() const { return m_dimensions; }
        const DataPoints& getDataPoints() const { return m_dataPoints; }
        DataPoints getRandomClusterCenters(int numOfClusters);
};