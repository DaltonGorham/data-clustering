#pragma once
#include <iostream>
#include <cstdlib>

class Utilities {
    public:
        static void validateArgs(int argc, char* argv[], int expectedArgs) {
            if (argc != expectedArgs) {
                std::cerr << "Usage: " << argv[0]
                    << " <input_file>"
                    << " <number_of_clusters>"
                    << " <max_iterations>"
                    << " <convergence_threshold>"
                    << " <number_of_runs>\n";
                std::exit(1);
            }
        }
};
