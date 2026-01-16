#include <iostream>
#include <string>
#include <filesystem>
#include "FileParser.h"
#include "Utilities.h"

const int ARGS = 6;
namespace fs = std::filesystem;

int main(int argc, char* argv[]) {
    Utilities::validateArgs(argc, argv, ARGS);
    fs::path basePath = fs::path(__FILE__).parent_path() / "phase1_data_sets";

    std::vector<std::string> lines = FileParser::parseFile((basePath / argv[1]).string());
    for (const auto& line : lines) {
        std::cout << line << std::endl;
    }
}
