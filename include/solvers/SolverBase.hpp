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


namespace gracfl
{
    /**
     * @class SolverBase
     * @brief Encapsulates configuration, graph loading, and execution of CFL-reachability analysis.
     *
     * Reads a normalized context-free grammar and graph according to provided configuration,
     * then applies the chosen CFL-reachability algorithm (BI, BW, or FW) in serial
     * or parallel mode.
     */
    class SolverBase 
    {
    public:
        /**
         * @brief Constructs a new SolverBase with specified configuration.
         * @param config Reference to a Config object containing solver settings (graph path, grammar path, mode, threads, etc.).
         * @throws std::runtime_error if grammar or graph cannot be loaded.
         */
        SolverBase() = default;

        /**
         * @brief Destructor: releases any allocated grammar or graph resources.
         */
        virtual ~SolverBase() = default;

        /**
         * @brief Executes the CFL-reachability algorithm on the processed graph.
         *
         * Depending on the model (gracfl or base) and mode (serial or parallel),
         * runs the appropriate solver routine to compute reachable pairs.
         */
        virtual void runCFL() = 0;

        virtual ull getEdgeCount() = 0;
        // virtual std::vector<std::vector<std::unordered_set<ull>>>& getHashset() = 0;
    };
}