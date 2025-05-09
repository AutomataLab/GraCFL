#pragma once

#include "utils/graphs/Graph.hpp"
#include "utils/graphs/Graph3DIn.hpp"
#include "utils/Edges.hpp"
#include "utils/Types.hpp"
#include "utils/Config.hpp"
#include "utils/Grammar.hpp"
#include "solvers/SolverBase.hpp"
#include "solvers/SolverBWGram.hpp"

namespace gracfl 
{
    /**
     * @class SolverBWGramParallel
     * @brief Parallel backward grammar-driven CFL solver.
     *
     * This class extends SolverBWGram to support multi-threaded execution using OpenMP.
     * It accelerates CFL reachability analysis by parallelizing each iteration over graph nodes.
     */
    class SolverBWGramParallel : public SolverBWGram 
    {
        uint numOfThreads_; ///< Number of threads to be used for parallel execution.
    public:
        /**
         * @brief Constructor for SolverBWGramParallel.
         * @param graphfilepath Path to the input graph file.
         * @param grammar Reference to the Grammar object.
         * @param numOfThreads Number of threads to use in parallel execution.
         */
        SolverBWGramParallel(std::string graphfilepath, Grammar& grammar, uint numOfThreads);

        /**
         * @brief Executes the CFL solver until convergence is achieved.
         */
        void runCFL() override;

        /**
         * @brief Executes a single parallel CFL iteration.
         *
         * Distributes grammar rule applications and hashset updates across multiple threads to
         * improve performance over the serial version.
         *
         * @param inEdges Incoming edges organized by label and destination node.
         * @param inHashset Reachability set (node × label → {source nodes}).
         * @param grammar2index Binary grammar productions (A → B).
         * @param grammar3indexLeft Ternary grammar productions (A → B C, left-associated, B -> list of (A, C)s).
         * @param labelSize Number of grammar nonterminals.
         * @param nodeSize Number of nodes in the graph.
         * @param terminate Flag indicating whether convergence has been reached.
         */
        void runSingleIterationParallel(
            std::vector<std::vector<TemporalVector>>& inEdges,
            std::vector<std::vector<std::unordered_set<ull>>>& inHashset,
            std::vector<std::vector<uint>>& grammar2index,
            std::vector<std::vector<std::pair<uint, uint>>>& grammar3indexLeft,
            uint labelSize,
            uint nodeSize,
            bool& terminate);
    };
}