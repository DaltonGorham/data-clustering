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
        std::string m_inputFile;
        int m_numOfPoints;
        int m_dimensions;
        DataPoints m_dataPoints;

        std::set<int> selectRandomIndices(int numOfClusters) const;
    public:
        Dataset(int numOfPoints, int dimensions, const DataPoints& dataPoints, const std::string& inputFile = "")
            : m_numOfPoints(numOfPoints), m_dimensions(dimensions), m_dataPoints(dataPoints), m_inputFile(inputFile) {}
        void printDataset() const;
        int getNumOfPoints() const { return m_numOfPoints; }
        int getDimensions() const { return m_dimensions; }
        const DataPoints& getDataPoints() const { return m_dataPoints; }
        const std::string& getInputFile() const { return m_inputFile; }
        DataPoints getRandomClusterCenters(int numOfClusters) const;
};