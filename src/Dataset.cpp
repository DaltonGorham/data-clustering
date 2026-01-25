/*
    Coding Standards: C++ Core Guidelines
    https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines
    C++ language standard version: C++20
*/
#include "../include/Dataset.h"
#include "../include/FileParser.h"
#include <sstream>
#include <fstream>
#include <ranges>
#include <random>
#include <set>
#include <filesystem>

namespace fs = std::filesystem;

Dataset FileParser::parseFileContents(const std::vector<std::string>& lines) {
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
    return Dataset(numOfPoints, dimensions, dataPoints);
}

std::set<int> Dataset::selectRandomIndices(int numOfClusters) {
    std::set<int> uniqueIndices;
    std::random_device randomDevice;
    std::mt19937 generator(randomDevice());
    std::uniform_int_distribution<int> distribution(0, m_numOfPoints - 1);
    while (uniqueIndices.size() < numOfClusters) {
        uniqueIndices.insert(distribution(generator));
    }
    return uniqueIndices;
}

void Dataset::setRandomClusterCenters(int numOfClusters) {
    std::set<int> selectedIndices = selectRandomIndices(numOfClusters);
    for (auto index : selectedIndices) {
        m_clusterCenters.push_back(m_dataPoints[index]);
    }
}

DataPoints Dataset::getRandomClusterCenters(int numOfClusters) {
    if (numOfClusters > m_numOfPoints) {
        std::cerr << "Error: Number of clusters requested: " << numOfClusters
                    << " exceeds number of data points: " << m_numOfPoints << "\n";
        std::exit(EXIT_FAILURE);
    }
    setRandomClusterCenters(numOfClusters);
    return m_clusterCenters;
}

void Dataset::outputClusterCenters(DataPoints clusterCenters, const std::string& inputFile) const {
    if (!fs::exists("output")) {
        fs::create_directory("output");
    }

    fs::path inputPath(inputFile);
    std::string outputPath = "output/" + inputPath.stem().string() + "_output.txt";
    std::ofstream outputFile(outputPath);

    if (!outputFile.is_open()) {
        std::cerr << "Error: Could not open output file: " << outputPath << "\n";
        return;
    }

    for (const auto& center : clusterCenters) {
        for (const auto& value : center) {
            std::cout << value << " ";
            outputFile << value << " ";
        }
        std::cout << "\n";
        outputFile << "\n";
    }
    outputFile.close();
}

void Dataset::printDataset() const {
    std::cout << "Number of Points: " << m_numOfPoints << "\n";
    std::cout << "Dimensions: " << m_dimensions << "\n";
    for (const auto& point : m_dataPoints) {
        for (const auto& value : point) {
            std::cout << value << " ";
        }
        std::cout << "\n";
    }
}