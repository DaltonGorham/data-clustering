#include "../include/Dataset.h"
#include "../include/FileParser.h"
#include <stdexcept>
#include <sstream>
#include <ranges>

Dataset FileParser::parseFileContents(const std::vector<std::string>& lines) {
    if (lines.empty()) {
        throw std::runtime_error("File is empty or has no valid data.");
    }

    // we expect the first line to contain metadata: number of points and dimensions
    std::istringstream headerStream(lines[0]);
    int numOfPoints, dimensions;
    headerStream >> numOfPoints >> dimensions;

    if (numOfPoints <= 0 || dimensions <= 0) {
        throw std::runtime_error("Invalid dataset metadata: number of points and dimensions must be positive integers.");
    }

    std::vector<std::vector<double>> dataPoints;
    // we dont need the header line anymore
    for (const auto& line : lines | std::views::drop(1)) {
        std::istringstream lineStream(line);
        std::vector<double> point;
        double value;
        while (lineStream >> value) {
            point.push_back(value);
        }
        dataPoints.push_back(point);
    }
    return Dataset(numOfPoints, dimensions, dataPoints);
}

std::vector<std::vector<double>> Dataset::selectRandomClusterCenters(int k) {
    if (k > m_numOfPoints) {
        throw std::runtime_error("Number of clusters cannot exceed number of data points.");
    }
    
    std::vector<std::vector<double>> centers;
    // we need to choose random indices. use actual points, dont create random points. 
    // use them from dataPoints. which is a vector of the data points.

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