#pragma once

#include "utils/graphs/Graph.hpp"
#include "utils/graphs/Graph3DIn.hpp"
#include "utils/Edges.hpp"
#include "utils/Types.hpp"
#include "utils/Config.hpp"
#include "utils/Grammar.hpp"
#include "solvers/SolverBase.hpp"
#include "solvers/SolverBWTopo.hpp"

namespace gracfl 
{
    /**
     * @class SolverBWTopoParallel
     * @brief A parallel extension of the backward topological CFL reachability solver.
     * 
     * Inherits from SolverBWTopo and enables parallel execution using OpenMP threads
     * to accelerate the context-free language (CFL) reachability analysis on labeled graphs.
     */
    class SolverBWTopoParallel : public SolverBWTopo 
    {
        uint numOfThreads_; ///< Number of threads to use for parallel execution.
    public:
        /**
         * @brief Constructor for SolverBWTopoParallel.
         * 
         * Initializes the solver with the input graph file path, grammar, and number of threads.
         * 
         * @param graphfilepath Path to the graph file to be loaded.
         * @param grammar Reference to the Grammar object used in the analysis.
         * @param numOfThreads Number of OpenMP threads to use during parallel execution.
         */
        SolverBWTopoParallel(std::string graphfilepath, Grammar& grammar, uint numOfThreads);

        /**
         * @brief Executes the full CFL reachability algorithm in parallel.
         * 
         * Overrides the base class method to incorporate multi-threading.
         */
        void runCFL() override;

        /**
         * @brief Executes a single iteration of the parallel CFL reachability algorithm.
         * 
         * Applies grammar rules and updates CFL-reachable sets in parallel across the graph.
         * 
         * @param inEdges Input edge lists indexed by grammar labels and destination vertices.
         * @param inHashset Data structure for storing reachability information.
         * @param grammar2index Maps unary grammar rules to index-based access.
         * @param grammar3index Maps binary grammar rules to index-based access.
         * @param labelSize Total number of unique grammar labels.
         * @param nodeSize Number of nodes in the graph.
         * @param terminate Flag indicating whether convergence has been reached.
         */
        void runSingleIterationParallel(
            std::vector<TemporalVectorWithLbldVtx>& inEdges,
            std::vector<std::vector<std::unordered_set<ull>>>& inHashset,
            std::vector<std::vector<uint>>& grammar2index,
            std::vector<std::vector<uint>>& grammar3index,
            uint labelSize,
            uint nodeSize,
            bool& terminate);
    };
}