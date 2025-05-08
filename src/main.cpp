#include <iostream>
#include <string>
#include <unordered_map>
#include <stdexcept>
#include <omp.h>
#include "solvers/Solver.hpp"
#include "utils/Config.hpp"

int main(int argc, char* argv[]) {
    try {
        gracfl::Config config(argc, argv);
        config.printConfigs();
        gracfl::Solver* solver = new gracfl::Solver(config);
        solver->solve();
        delete solver;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n\n";
        gracfl::Config::printUsage(argv[0]);
        return 1;
    }

    return 0;
}

