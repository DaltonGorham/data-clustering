/*
    Coding Standards: C++ Core Guidelines
    https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines
    C++ language standard version: C++20
*/
#pragma once
#include "Dataset.h"
#include <vector>

struct KMeansResult {
    int numOfClusters = 0;
    double bestSSE = 0.0;
    double calinskiHarabaszIndex = 0.0;
    double silhouetteWidth = 0.0;
    DataPoints bestCenters;
    std::vector<int> bestClusterAssignments;
};
