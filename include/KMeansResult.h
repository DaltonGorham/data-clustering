/*
    Coding Standards: C++ Core Guidelines
    https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines
    C++ language standard version: C++20
*/
#pragma once
#include "Dataset.h"
#include <vector>

struct KMeansResult {
    double sse = 0.0;
    double randIndex = 0.0;
    double jaccardIndex = 0.0;
    DataPoints centers;
    std::vector<int> clusterAssignments;
};
