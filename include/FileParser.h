#pragma once
#include <string>
#include <vector>
#include "Dataset.h"

class FileParser {
    public:
        static std::vector<std::string> readFile(const std::string& filePath);
        static Dataset parseFileContents(const std::vector<std::string>& lines);
};
