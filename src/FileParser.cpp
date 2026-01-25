/*
    Coding Standards: C++ Core Guidelines
    https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines
    C++ language standard version: C++20
*/
#include "../include/FileParser.h"
#include <fstream>

std::vector<std::string> FileParser::readFile(const std::string& filePath) {
    std::vector<std::string> lines;
    std::ifstream file(filePath);
    
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file: " << filePath << "\n";
        std::exit(1);
    }
    
    std::string line;
    while (std::getline(file, line)) {
        lines.push_back(line);
    }
    
    return lines;
}
