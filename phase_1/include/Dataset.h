#pragma once
#include <iostream>
#include <vector>
#include <string>

class Dataset {
    private:
        int m_numOfPoints;
        int m_dimensions;
        std::vector<std::vector<double>> m_dataPoints;
        std::vector<std::vector<double>> m_clusterCenters;

        std::vector<std::vector<double>> selectRandomClusterCenters(int k);
    public:
        Dataset(int numOfPoints, int dimensions, const std::vector<std::vector<double>>& dataPoints)
            : m_numOfPoints(numOfPoints), m_dimensions(dimensions), m_dataPoints(dataPoints) {}
        void printDataset() const;
        int getNumOfPoints() const { return m_numOfPoints; }
        int getDimensions() const { return m_dimensions; }
        const std::vector<std::vector<double>>& getDataPoints() const { return m_dataPoints; }

};