#pragma once

#include <iostream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <stdexcept>
#include <omp.h>
#include "utils/Edges.hpp"
#include "utils/Grammar.hpp"
#include "utils/graphs/Graph.hpp"
#include "utils/graphs/Graph3DBi.hpp"
#include "utils/graphs/Graph3DOut.hpp"
#include "utils/graphs/Graph3DIn.hpp"
#include "utils/Types.hpp"
#include "utils/Config.hpp"
#include "solvers/SolverBIGram.hpp"
#include "solvers/SolverBITopo.hpp"
#include "solvers/SolverBWGram.hpp"
#include "solvers/SolverBWTopo.hpp"
#include "solvers/SolverFWGram.hpp"
#include "solvers/SolverFWTopo.hpp"
#include "solvers/SolverFWGramParallel.hpp"
#include "solvers/SolverFWTopoParallel.hpp"
#include "solvers/SolverBWGramParallel.hpp"
#include "solvers/SolverBWTopoParallel.hpp"
#include "solvers/SolverBase.hpp"


namespace gracfl
{
    /**
     * @class Solver
     * @brief Encapsulates configuration, graph loading, and execution of CFL-reachability analysis.
     *
     * Reads a normalized context-free grammar and graph according to provided configuration,
     * then applies the chosen CFL-reachability algorithm (BI, BW, or FW) in serial
     * or parallel mode.
     */
    class Solver 
    {
        /// Solver configuration parameters
        Config& config_;
        /// Pointer to loaded grammar
        Grammar* grammar_;
        /// Pointer to the solver based on the config
        SolverBase* solver_;
    public:
        /**
         * @brief Constructs a new Solver with specified configuration.
         * @param config Reference to a Config object containing solver settings (graph path, grammar path, mode, threads, etc.).
         * @throws std::runtime_error if grammar or graph cannot be loaded.
         */
        Solver(Config& config);

        /**
         * @brief Destructor: releases any allocated grammar or graph resources.
         */
        ~Solver();

        SolverBase* selectSolver();

        /**
         * @brief Executes the CFL-reachability algorithm on the processed graph.
         *
         * Depending on the model (gracfl or base) and mode (serial or parallel),
         * runs the appropriate solver routine to compute reachable pairs.
         */
        void solve();

        std::vector<std::vector<std::unordered_set<ull>>>& getGraph();
    };
}