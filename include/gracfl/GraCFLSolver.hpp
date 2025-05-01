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
#include "gracfl/BIGracfl.hpp"
#include "gracfl/BWGracfl.hpp"
#include "gracfl/FWGracfl.hpp"
#include "gracfl/FWGracflParallel.hpp"


namespace gracfl
{
    /**
     * @class GraCFLSolver
     * @brief Encapsulates configuration, graph loading, and execution of CFL-reachability analysis.
     *
     * Reads a normalized context-free grammar and graph according to provided configuration,
     * then applies the chosen CFL-reachability algorithm (BI, BW, or FW) in serial
     * or parallel mode.
     */
    class GraCFLSolver 
    {
        /// Solver configuration parameters
        Config config_;
        /// Pointer to loaded grammar
        Grammar* grammar_;
        /// Pointer to loaded graph representation
        Graph* graph_;
    public:
        /**
         * @brief Constructs a new GraCFLSolver with specified configuration.
         * @param config Reference to a Config object containing solver settings (graph path, grammar path, mode, threads, etc.).
         * @throws std::runtime_error if grammar or graph cannot be loaded.
         */
        GraCFLSolver(Config& config);

        /**
         * @brief Destructor: releases any allocated grammar or graph resources.
         */
        ~GraCFLSolver();

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