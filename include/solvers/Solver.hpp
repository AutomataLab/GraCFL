#pragma once

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
     * @brief Main driver class that encapsulates configuration, grammar, and execution of CFL-reachability analysis.
     * 
     * Based on the configuration provided, this class loads the grammar, initializes the appropriate solver
     * (e.g., FW, BW, BI with gram/topo driven strategy, in serial or parallel mode), and executes the CFL-reachability algorithm.
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
         * @param config Reference to a Config object containing solver settings.
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
         * runs the appropriate solver routine to compute reachable pairs.
         */
        void solve();

        /**
         * @brief Provides access to the graph reachability results stored inside the solver.
         * 
         * This allows inspection or post-processing of the reachability sets computed
         * by the selected solver.
         * 
         * @return Reference to the 2D vector of unordered sets representing reachable nodes per label. The first index is the vertex ID,
         *         and the second index is the label ID. Each unordered set contains the reachable vertex IDs.
         */
        std::vector<std::vector<std::unordered_set<ull>>> getGraph();
    };
}