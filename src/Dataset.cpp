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

    // we expect the first line to contain metadata: num of points, dimensions + 1, true num of clusters
    std::istringstream headerStream(lines[0]);
    int numOfPoints, actualDimensions, trueNumOfClusters;
    headerStream >> numOfPoints >> actualDimensions >> trueNumOfClusters;
    actualDimensions--; // header stores attributes + 1

    if (numOfPoints <= 0 || actualDimensions <= 0 || trueNumOfClusters <= 0) {
        std::cerr << "Error: Invalid dataset metadata: number of points: " << numOfPoints
                  << ", dimensions: " << actualDimensions
                  << ", true clusters: " << trueNumOfClusters << " must be positive integers.\n";
        std::exit(EXIT_FAILURE);
    }

    DataPoints dataPoints;
    std::vector<int> trueClusterAssignments;

    for (const auto& line : lines | std::views::drop(1)) {
        std::istringstream lineStream(line);
        std::vector<double> point;
        double value;
        while (lineStream >> value) {
            if (static_cast<int>(point.size()) == actualDimensions) {
                trueClusterAssignments.push_back(static_cast<int>(value));
                continue; // dont add the true cluster into the data point
            }
            point.push_back(value);
        }
        dataPoints.push_back(point);
    }

    if (static_cast<int>(dataPoints.size()) != numOfPoints) {
        std::cerr << "Error: Mismatch between declared number of points: " << numOfPoints
                  << " and actual data points read: " << dataPoints.size() << "\n";
        std::exit(EXIT_FAILURE);
    }
    return Dataset(
        numOfPoints, actualDimensions, trueNumOfClusters,
        trueClusterAssignments, dataPoints, inputFile
    );
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
    
    if (sse == 0.0) return 0.0;

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

    return static_cast<double>(m_numOfPoints - numOfClusters) / (numOfClusters - 1) * sb / sse;
}

// https://dataminingbook.info/book_html/chap17/book.html pg. 446
// s_i = (mu_out_min - mu_in) / max(mu_in, mu_out_min)
// SC  = (1/n) * sum(s_i)
double Dataset::silhouetteWidth(
    const DataPoints& clusterCenters, 
    const std::vector<int>& clusterAssignments
) const {
    
    auto euclideanDistance = [&](int i, int j) {
        double dist = 0.0;
        for (int dim = 0; dim < m_dimensions; dim++) {
            dist += (m_dataPoints[i][dim] - m_dataPoints[j][dim]) * (m_dataPoints[i][dim] - m_dataPoints[j][dim]);
        }
        return sqrt(dist);
    };

    int numOfClusters = clusterCenters.size();
    std::vector<int> clusterSizes(numOfClusters, 0);

    for (int assignment : clusterAssignments) {
        clusterSizes[assignment]++;
    }

    double silhouetteWidth = 0.0;
    // TODO: O(n^2), this can be optimized by precomputing distances
    for (int pointIndex = 0; pointIndex < m_numOfPoints; pointIndex++) {
        int cluster = clusterAssignments[pointIndex];
        double meanDistanceToOwnCluster = 0.0;
        std::vector<double> sumDistanceToOtherCluster(numOfClusters, 0.0);

        for (int neighborIndex = 0; neighborIndex < m_numOfPoints; neighborIndex++) {
            if (neighborIndex == pointIndex) continue;
            
            double distanceFromNeighbor = euclideanDistance(pointIndex, neighborIndex);

            if (clusterAssignments[neighborIndex] == cluster) {
                meanDistanceToOwnCluster += distanceFromNeighbor;
            } else {
                sumDistanceToOtherCluster[clusterAssignments[neighborIndex]] += distanceFromNeighbor;
            }
        }

        if (clusterSizes[cluster] > 1) {
            meanDistanceToOwnCluster /= (clusterSizes[cluster] - 1);
        }

        double minClusterDistance = std::numeric_limits<double>::max();
        for (int i = 0; i < numOfClusters; i++) {
            // only care for other clusters, not the one the point belongs to
            if (i != cluster && clusterSizes[i] > 0) {
                double meanDist = sumDistanceToOtherCluster[i] / clusterSizes[i];
                if (meanDist < minClusterDistance) {
                    minClusterDistance = meanDist;
                }
            }
        }

        // accumulate s_i for each point
        silhouetteWidth += (minClusterDistance - meanDistanceToOwnCluster) / std::max(meanDistanceToOwnCluster, minClusterDistance);
    }
    // close to +1 indicates a good clustering and closer to -1 indicates it might be a bad clustering
    return silhouetteWidth / m_numOfPoints;
}

// https://dataminingbook.info/book_html/chap17/book.html pg. 436-437
std::pair<double, double> Dataset::randAndJaccardIndex(const std::vector<int>& clusterAssignments) const {
    int truePositive = 0, falseNegative = 0, falsePositive = 0, trueNegative = 0;
    int numOfPairs = m_numOfPoints * (m_numOfPoints - 1) / 2;
    
    for (int i = 0; i < m_numOfPoints; i++) {
        for (int j = i + 1; j < m_numOfPoints; j++) {
            bool sameClusterAssignment = (clusterAssignments[i] == clusterAssignments[j]);
            bool sameTrueCluster = (m_trueClusterAssignments[i] == m_trueClusterAssignments[j]);

            if (sameClusterAssignment && sameTrueCluster) truePositive++;
            else if (!sameClusterAssignment && sameTrueCluster) falseNegative++;
            else if (sameClusterAssignment && !sameTrueCluster) falsePositive++;
            else trueNegative++;
        }
    }

    double rand = static_cast<double>(truePositive + trueNegative) / numOfPairs;
    double jaccard = static_cast<double>(truePositive) / (truePositive + falseNegative + falsePositive);
    return {rand, jaccard};
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