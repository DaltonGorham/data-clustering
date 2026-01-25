/*
    Coding Standards: C++ Core Guidelines
    https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines
    C++ language standard version: C++20
*/
#pragma once
#include <string>
#include <vector>
#include "Dataset.h"

class FileParser {
    public:
        static std::vector<std::string> readFile(const std::string& filePath);
        static Dataset parseFileContents(const std::vector<std::string>& lines);
};
