/*
    Coding Standards: C++ Core Guidelines
    https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines
    C++ language standard version: C++20
*/
#include <iostream>
#include <string>
#include "../include/FileParser.h"
#include "../include/Utilities.h"

const int ARGS = 6;

int main(int argc, char* argv[]) {
    Utilities::validateArgs(argc, argv, ARGS);
    auto lines = FileParser::readFile(argv[1]);
    Dataset dataset = FileParser::parseFileContents(lines);
    dataset.printDataset();

    // // debug print. remove later
    // for (const auto& line : lines) {
    //     std::cout << line << std::endl;
    // }
}
