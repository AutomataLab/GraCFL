#include <iostream>
#include <string>
#include <unordered_map>
#include <stdexcept>
#include <omp.h>
#include "solvers/SolverGraCFL.hpp"
#include "utils/Config.hpp"

int main(int argc, char* argv[]) {
    try {
        gracfl::Config config(argc, argv);
        config.printConfigs();
        gracfl::SolverGraCFL solver(config);
        solver.solveCFL();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n\n";
        gracfl::Config::printUsage(argv[0]);
        return 1;
    }

    return 0;
}

