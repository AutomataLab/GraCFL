#pragma once

#include "utils/graphs/Graph.hpp"
#include "utils/graphs/Graph3DOut.hpp"
#include "utils/Edges.hpp"
#include "utils/Types.hpp"
#include "utils/Config.hpp"
#include "utils/Grammar.hpp"
#include "solvers/SolverBase.hpp"
#include "solvers/SolverFWTopo.hpp"

namespace gracfl 
{
    /**
     * @class SolverFWTopoParallel
     * @brief A parallel extension of the forward topological CFL reachability solver.
     * 
     * Inherits from SolverFWTopo and enables parallel execution using OpenMP threads
     * to accelerate the context-free language (CFL) reachability analysis on labeled graphs.
     */
    class SolverFWTopoParallel : public SolverFWTopo 
    {
        uint numOfThreads_; ///< Number of threads to use for parallel execution.
    public:
        /**
         * @brief Constructor for SolverFWTopoParallel.
         * 
         * Initializes the solver with the input graph file path, grammar, and number of threads.
         * 
         * @param graphfilepath Path to the graph file to be loaded.
         * @param grammar Reference to the Grammar object used in the analysis.
         * @param numOfThreads Number of OpenMP threads to use during parallel execution.
         */
        SolverFWTopoParallel(std::string graphfilepath, Grammar& grammar, uint numOfThreads);

        /**
         * @brief Executes the full parallel forward-directional CFL-reachability analysis.
         */
        void runCFL() override;

        /**
         * @brief Performs one iteration of edge derivation.
         * 
         * In this iteration, derivations are parallelized across vertices
         * to leverage multi-threaded performance.
         * 
         * @param outEdges Outgoing edges organized by label and source vertex.
         * @param hashset Data structure to store reachability results.
         * @param grammar2index Index-based access for unary grammar rules.
         * @param grammar3index Index-based access for binary grammar rules.
         * @param labelSize Number of unique grammar labels.
         * @param nodeSize Total number of nodes in the graph.
         * @param terminate Flag indicating whether convergence has been reached.
         */
        void runSingleIterationParallel(
            std::vector<TemporalVectorWithLbldVtx>& outEdges,
            std::vector<std::vector<std::unordered_set<ull>>>& hashset,
            std::vector<std::vector<uint>>& grammar2index,
            std::vector<std::vector<uint>>& grammar3index,
            uint labelSize,
            uint nodeSize,
            bool& terminate);
    };
}