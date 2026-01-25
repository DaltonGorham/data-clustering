/*
    Coding Standards: C++ Core Guidelines
    https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines
    C++ language standard version: C++20
*/
#pragma once
#include "Dataset.h"

class KMeans {
    private:
        int m_numOfClusters;
        int m_maxIterations;
        double m_convergenceThreshold;
        double m_SSE;
        DataPoints m_clusterCenters;
    public:
        KMeans(int numOfClusters, int maxIterations, double convergenceThreshold)
            : m_numOfClusters(numOfClusters), m_maxIterations(maxIterations), m_convergenceThreshold(convergenceThreshold) {}
        const double getSSE() const { return m_SSE; }
        
};