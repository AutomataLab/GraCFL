#pragma once

#include "utils/graphs/Graph.hpp"
#include "utils/graphs/Graph3DOut.hpp"
#include "utils/Edges.hpp"
#include "utils/Types.hpp"
#include "utils/Config.hpp"

namespace gracfl 
{
    /**
     * @class SolverFWGram
     * @brief  Forward directional CFL-reachability graph implementation and analysis using grammar-driven travesal and sliding pointers.
     * 
     * Inherits from Graph and adds support for forward directional edge derivations.
     * Maintains out-edges adjacency list, as well as a hashset to avoid duplicates.
     */
    class SolverFWGram : public SolverBase 
    {
        Grammar& grammar_;
        Graph3DOut& graph_;
    public:
        SolverFWGram(Grammar& grammar, Graph3DOut& graph);

        void runCFL() override;

        /**
         * @brief Performs one iteration of edge derivation.
         * @param grammar   Grammar rules for edge derivations.
         * @param terminate Flag set to false if new edges were added.
         */
        void runSingleIteration(bool& terminate);

        /**
         * @brief Adds self-loop edges (epsilon rules) for all nodes.
         * @param grammar Grammar rules for edge derivations.
         */
        void addSelfEdges();

    };
}