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
     * @brief  Parallel Forward directional CFL-reachability graph implementation and analysis using grammar-driven travesal and sliding pointers.
     * 
     * Inherits from SolverFWGram and adds support for parallel forward directional edge derivations.
     */
    class SolverBWGramParallel : public SolverBWGram 
    {
        uint numOfThreads_;
    public:
        SolverBWGramParallel(std::string graphfilepath, Grammar& grammar, uint numOfThreads);

        /**
         * @brief Executes the full parallel forward-directional CFL-reachability analysis.
         * @param grammar Grammar rules for generating new edges.
         */
        void runCFL() override;

         /**
         * @brief Performs one iteration of edge derivation.
         * 
         * In this iteration, derivations are parallelized across vertices
         * to leverage multi-threaded performance.
         * @param grammar   Grammar rules for edge derivations.
         * @param terminate Flag set to false if new edges were added.
         */
        void runSingleIterationParallel(
            std::vector<std::vector<TemporalVector>>& inEdges,
            std::vector<std::vector<std::unordered_set<ull>>>& hashset,
            std::vector<std::vector<uint>>& grammar2index,
            std::vector<std::vector<std::pair<uint, uint>>>& grammar3indexLeft,
            uint labelSize,
            uint nodeSize,
            bool& terminate);
    };
}