#pragma once

#include <iostream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <stdexcept>
#include <omp.h>
#include "utils/Edges.hpp"
#include "utils/Grammar.hpp"
#include "utils/Graph.hpp"
#include "utils/Types.hpp"
#include "utils/Config.hpp"
#include "solvers/SolverBIGram.hpp"
#include "solvers/SolverBWGram.hpp"
#include "solvers/SolverFWGram.hpp"
#include "solvers/SolverFWGramParallel.hpp"


namespace gracfl
{
    /**
     * @class SolverGraCFL
     * @brief Encapsulates configuration, graph loading, and execution of CFL-reachability analysis.
     *
     * Reads a normalized context-free grammar and graph according to provided configuration,
     * then applies the chosen CFL-reachability algorithm (BI, BW, or FW) in serial
     * or parallel mode.
     */
    class SolverGraCFL 
    {
        /// Solver configuration parameters
        Config config_;
        /// Pointer to loaded grammar
        Grammar* grammar_;
        /// Pointer to loaded graph representation
        Graph* graph_;
    public:
        /**
         * @brief Constructs a new SolverGraCFL with specified configuration.
         * @param config Reference to a Config object containing solver settings (graph path, grammar path, mode, threads, etc.).
         * @throws std::runtime_error if grammar or graph cannot be loaded.
         */
        SolverGraCFL(Config& config);

        /**
         * @brief Destructor: releases any allocated grammar or graph resources.
         */
        ~SolverGraCFL();

        /**
         * @brief Processes the input graph according to the loaded grammar and model.
         *
         * This function builds the graph object (BI, BW, or FW variant)
         * based on the configuration and grammar, preparing it for solving.
         * @return Pointer to the fully initialized Graph instance.
         */
        Graph* processGraph();

        /**
         * @brief Executes the CFL-reachability algorithm on the processed graph.
         *
         * Depending on the model (gracfl or base) and mode (serial or parallel),
         * runs the appropriate solver routine to compute reachable pairs.
         */
        void solveCFL();
    };
}