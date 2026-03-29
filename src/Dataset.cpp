/*
    Coding Standards: C++ Core Guidelines
    https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines
    C++ language standard version: C++20
*/
#include "../include/Dataset.h"
#include "../include/FileParser.h"
#include <fstream>
#include <sstream>
#include <ranges>
#include <random>
#include <set>


Dataset FileParser::parseFileContents(const std::vector<std::string>& lines, const std::string& inputFile) {
    if (lines.empty()) {
        std::cerr << "Error: Input file is empty or improperly formatted.\n";
        std::exit(EXIT_FAILURE);
    }

    // we expect the first line to contain metadata: number of points and dimensions
    std::istringstream headerStream(lines[0]);
    int numOfPoints, dimensions;
    headerStream >> numOfPoints >> dimensions;

    if (numOfPoints <= 0 || dimensions <= 0) {
        std::cerr << "Error: Invalid dataset metadata: number of points: " << numOfPoints 
                  << ", dimensions: " << dimensions << " must be positive integers.\n";
        std::exit(EXIT_FAILURE);
    }

    DataPoints dataPoints;

    for (const auto& line : lines | std::views::drop(1)) {
        std::istringstream lineStream(line);
        std::vector<double> point;
        double value;
        while (lineStream >> value) {
            point.push_back(value);
        }
        dataPoints.push_back(point);
    }

    if (dataPoints.size() != numOfPoints) {
        std::cerr << "Error: Mismatch between declared number of points: " << numOfPoints 
                  << " and actual data points read: " << dataPoints.size() << "\n";
        std::exit(EXIT_FAILURE);
    }
    return Dataset(numOfPoints, dimensions, dataPoints, inputFile);
}


std::set<int> Dataset::selectRandomIndices(int numOfClusters) const {
    std::set<int> uniqueIndices;
    std::random_device randomDevice;
    std::mt19937 generator(randomDevice());
    std::uniform_int_distribution<int> distribution(0, m_numOfPoints - 1);
    while (uniqueIndices.size() < numOfClusters) {
        uniqueIndices.insert(distribution(generator));
    }
    return uniqueIndices;
}


DataPoints Dataset::getRandomClusterCenters(int numOfClusters) const {
    if (numOfClusters > m_numOfPoints) {
        std::cerr << "Error: Number of clusters requested: " << numOfClusters
                    << " exceeds number of data points: " << m_numOfPoints << "\n";
        std::exit(EXIT_FAILURE);
    }
    
    std::set<int> selectedIndices = selectRandomIndices(numOfClusters);
    DataPoints clusterCenters;
    for (auto index : selectedIndices) {
        clusterCenters.push_back(m_dataPoints[index]);
    }
    return clusterCenters;
}


DataPoints Dataset::getRandomPartitionCenters(int numOfClusters) const {
    if (numOfClusters > m_numOfPoints) {
        std::cerr << "Error: Number of clusters requested: " << numOfClusters
                    << " exceeds number of data points: " << m_numOfPoints << "\n";
        std::exit(EXIT_FAILURE);
    }

    std::vector<int> clusterCenterIndices(m_numOfPoints);
    std::mt19937 generator(std::random_device{}());
    std::uniform_int_distribution<int> distribution(0, numOfClusters - 1);

    for (int point = 0; point < m_numOfPoints; point++) {
        clusterCenterIndices[point] = distribution(generator);
    }

    DataPoints clusterCenters(numOfClusters, std::vector<double>(m_dimensions, 0.0));
    std::vector<int> counts(numOfClusters, 0);
    
    for (int pointIndex = 0; pointIndex < clusterCenterIndices.size(); pointIndex++) {
        int cluster = clusterCenterIndices[pointIndex];
        counts[cluster]++;
        for (int d = 0; d < m_dimensions; d++) {
            clusterCenters[cluster][d] += m_dataPoints[pointIndex][d];
        }
    }

    for (int cluster = 0; cluster < numOfClusters; cluster++) {
        for (int d = 0; d < m_dimensions; d++) {
            clusterCenters[cluster][d] /= counts[cluster];
        }
    }
    
    return clusterCenters;
}


// normalize each column to the range [0, 1] using min-max normalization
// formula: v' = (v - min) / (max - min)
void Dataset::normalize() {
    for (int dim = 0; dim < m_dimensions; dim++) {
        double minVal = m_dataPoints[0][dim];
        double maxVal = m_dataPoints[0][dim];

        for (const auto& point : m_dataPoints) {
            if (point[dim] < minVal) minVal = point[dim];
            if (point[dim] > maxVal) maxVal = point[dim];
        }

        for (auto& point : m_dataPoints) {
            if (maxVal - minVal == 0) {
                point[dim] = 0.0; // avoid division by zero
            } else {
                point[dim] = (point[dim] - minVal) / (maxVal - minVal);
            }
        }
    }
}

/* 
https://dataminingbook.info/book_html/chap17/book.html 17.7
CH(k) = tr(SB) / (k −1) tr(SW) / (n −k) = (n −k) / (k −1) * tr(SB) / tr(SW)
SW: SSE
SB: The sum of the diagonal elements
*/
double Dataset::calinskiHarabaszIndex(
    const DataPoints& clusterCenters, 
    const std::vector<int>& clusterAssignments,
    double sse
) const {
    int numOfClusters = clusterCenters.size();
    std::vector<double> meanOfDataSet(m_dimensions, 0.0);
    std::vector<int> clusterSizes(numOfClusters, 0);
    
    for (int dim = 0; dim < m_dimensions; dim++) {
        for (const auto& point : m_dataPoints) {
            meanOfDataSet[dim] += point[dim];
        }
        meanOfDataSet[dim] /= m_numOfPoints;
    }

    // grab cluster sizes for SB calculation. n,i is the number of points in cluster i
    for (int assignment : clusterAssignments) {
        clusterSizes[assignment]++;
    }

    double sb = 0.0;

    for (int cluster = 0; cluster < numOfClusters; cluster++) {
        for (int dim = 0; dim < m_dimensions; dim++) {
            sb += clusterSizes[cluster] * (clusterCenters[cluster][dim] - meanOfDataSet[dim]) * (clusterCenters[cluster][dim] - meanOfDataSet[dim]);
        }
    }

    if (sse == 0.0) return 0.0; 
    return static_cast<double>(m_numOfPoints - numOfClusters) / (numOfClusters - 1) * sb / sse;
}


void Dataset::printDataset() const {
    std::ofstream outFile(m_inputFile + "_normalized.txt");
    std::cout << "Number of Points: " << m_numOfPoints << "\n";
    std::cout << "Dimensions: " << m_dimensions << "\n";
    for (const auto& point : m_dataPoints) {
        for (const auto& value : point) {
            std::cout << value << " ";
            outFile << value << " ";
        }
        std::cout << "\n";
        outFile << "\n";
    }
}