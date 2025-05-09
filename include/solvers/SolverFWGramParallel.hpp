#pragma once

#include "utils/graphs/Graph.hpp"
#include "utils/graphs/Graph3DOut.hpp"
#include "utils/Edges.hpp"
#include "utils/Types.hpp"
#include "utils/Config.hpp"
#include "utils/Grammar.hpp"
#include "solvers/SolverBase.hpp"
#include "solvers/SolverFWGram.hpp"

namespace gracfl 
{
    /**
     * @class SolverFWGramParallel
     * @brief  Parallel Forward directional CFL-reachability graph implementation and analysis using grammar-driven travesal and sliding pointers.
     * 
     * Inherits from SolverFWGram and adds support for parallel forward directional edge derivations.
     */
    class SolverFWGramParallel : public SolverFWGram 
    {
        uint numOfThreads_; ///< Number of threads to be used for parallel execution
    public:
        /**
         * @brief Constructor for SolverFWGramParallel.
         * 
         * Initializes the solver with the input graph file path, grammar, and number of threads.
         * 
         * @param graphfilepath Path to the graph file to be loaded.
         * @param grammar Reference to the Grammar object used in the analysis.
         * @param numOfThreads Number of OpenMP threads to use during parallel execution.
         */
        SolverFWGramParallel(std::string graphfilepath, Grammar& grammar, uint numOfThreads);

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
         * @param grammar3indexLeft Index-based access for binary grammar rules (left-child driven).
         * @param labelSize Number of unique grammar labels.
         * @param nodeSize Total number of nodes in the graph.
         * @param terminate Flag indicating whether convergence has been reached.
         */
        void runSingleIterationParallel(
            std::vector<std::vector<TemporalVector>>& outEdges,
            std::vector<std::vector<std::unordered_set<ull>>>& hashset,
            std::vector<std::vector<uint>>& grammar2index,
            std::vector<std::vector<std::pair<uint, uint>>>& grammar3indexLeft,
            uint labelSize,
            uint nodeSize,
            bool& terminate);
    };
}