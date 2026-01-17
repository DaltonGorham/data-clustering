#include <iostream>
#include <string>
#include "../include/FileParser.h"
#include "../include/Utilities.h"

const int ARGS = 6;

int main(int argc, char* argv[]) {
    Utilities::validateArgs(argc, argv, ARGS);
    std::vector<std::string> lines = FileParser::parseFile(argv[1]);
    
    for (const auto& line : lines) {
        std::cout << line << std::endl;
    }
}
