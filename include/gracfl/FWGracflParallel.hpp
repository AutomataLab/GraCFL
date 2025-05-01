#pragma once

#include "utils/Graph.hpp"
#include "utils/Edges.hpp"
#include "utils/Types.hpp"
#include "utils/Config.hpp"
#include "FWGracfl.hpp"

namespace gracfl 
{
    /**
     * @class FWGracflParallel
     * @brief  Parallel Forward directional CFL-reachability graph implementation and analysis using grammar-driven travesal and sliding pointers.
     * 
     * Inherits from FWGracfl and adds support for parallel forward directional edge derivations.
     */
    class FWGracflParallel : public FWGracfl 
    {
    public:
        /**
         * @brief Constructor passes data to FWCFLGraph and that allocates adjacency list + hashset, and reads initial edges.
         * @param config Solver configurations.
         * @param grammar       Grammar describing the CFL rules.
         */
        FWGracflParallel(Config& config, const Grammar& grammar);

        /**
         * @brief Executes the full parallel forward-directional CFL-reachability analysis.
         * @param grammar Grammar rules for generating new edges.
         */
        void solve(const Grammar& grammar) override;

         /**
         * @brief Performs one iteration of edge derivation.
         * 
         * In this iteration, derivations are parallelized across vertices
         * to leverage multi-threaded performance.
         * @param grammar   Grammar rules for edge derivations.
         * @param terminate Flag set to false if new edges were added.
         */
        void singleIterationParallelLabel(const Grammar& grammar, bool& terminate, uint numOfThreads);

        /**
         * @brief Performs one iteration of edge derivation.
         * 
         * In this iteration, derivations are parallelized across vertices
         * to leverage multi-threaded performance.
         * @param grammar   Grammar rules for edge derivations.
         * @param terminate Flag set to false if new edges were added.
         */
        void singleIterationParallel(const Grammar& grammar, bool& terminate, uint numOfThreads);
    };
}