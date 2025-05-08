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
     * @brief  Parallel Forward directional CFL-reachability graph implementation and analysis using grammar-driven travesal and sliding pointers.
     * 
     * Inherits from SolverBWTopo and adds support for parallel forward directional edge derivations.
     */
    class SolverBWTopoParallel : public SolverBWTopo 
    {
        uint numOfThreads_;
    public:
        SolverBWTopoParallel(std::string graphfilepath, Grammar& grammar, uint numOfThreads);

        /**
         * @brief Executes the full parallel forward-directional CFL-reachability analysis.
         * @param grammar Grammar rules for generating new edges.
         */
        void runCFL() override;

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