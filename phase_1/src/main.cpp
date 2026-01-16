#include <iostream>
#include <string>
#include <filesystem>
#include "../include/FileParser.h"
#include "../include/Utilities.h"

const int ARGS = 6;
namespace fs = std::filesystem;

int main(int argc, char* argv[]) {
    Utilities::validateArgs(argc, argv, ARGS);
    fs::path basePath = fs::path(__FILE__).parent_path().parent_path() / "data";

    std::vector<std::string> lines = FileParser::parseFile((basePath / argv[1]).string());
    for (const auto& line : lines) {
        std::cout << line << std::endl;
    }
}
