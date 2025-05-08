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
     * @brief  Parallel Forward directional CFL-reachability graph implementation and analysis using grammar-driven travesal and sliding pointers.
     * 
     * Inherits from SolverFWTopo and adds support for parallel forward directional edge derivations.
     */
    class SolverFWTopoParallel : public SolverFWTopo 
    {
        uint numOfThreads_;
    public:
        SolverFWTopoParallel(std::string graphfilepath, Grammar& grammar, uint numOfThreads);

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
            std::vector<TemporalVectorWithLbldVtx>& outEdges,
            std::vector<std::vector<std::unordered_set<ull>>>& hashset,
            std::vector<std::vector<uint>>& grammar2index,
            std::vector<std::vector<uint>>& grammar3index,
            uint labelSize,
            uint nodeSize,
            bool& terminate);
    };
}