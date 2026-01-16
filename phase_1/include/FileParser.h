#pragma once
#include <string>
#include <vector>

class FileParser {
    public:
        static std::vector<std::string> parseFile(const std::string& filePath);
};
