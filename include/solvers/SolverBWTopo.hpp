#pragma once

#include "utils/graphs/Graph.hpp"
#include "utils/graphs/Graph2DIn.hpp"
#include "utils/Edges.hpp"
#include "utils/Types.hpp"
#include "utils/Config.hpp"
#include "utils/Grammar.hpp"
#include "solvers/SolverBase.hpp"

namespace gracfl 
{
    /**
     * @class SolverBWTopo
     * @brief  Bidirectional CFL-reachability graph implementation and analysis using grammar-driven travesal and sliding pointers.
     * 
     * Inherits from Graph and adds support for bidirectional edge derivations.
     * Maintains separate out-edges and in-edges adjacency lists, as well as a hashset to avoid duplicates.
     */
    class SolverBWTopo : public SolverBase
    {
    protected:
        Grammar& grammar_;
        Graph2DIn* graph_;
    public:
        SolverBWTopo(std::string graphfilepath, Grammar& grammar);
        ~SolverBWTopo();

        /**
         * @brief Executes the full bidirectional CFL-reachability analysis.
         * @param grammar Grammar rules for generating new edges.
         */
        void runCFL() override;

        /**
         * @brief Performs one iteration of edge derivation.
         * @param grammar   Grammar rules for edge derivations.
         * @param terminate Flag set to false if new edges were added.
         */
        void runSingleIteration(
            std::vector<TemporalVectorWithLbldVtx>& inEdges,
            std::vector<std::vector<std::unordered_set<ull>>>& inHashset,
            std::vector<std::vector<uint>>& grammar2index,
            std::vector<std::vector<uint>>& grammar3index,
            uint labelSize,
            uint nodeSize,
            bool& terminate);

        /**
         * @brief Adds self-loop edges (epsilon rules) for all nodes.
         * @param grammar Grammar rules for edge derivations.
         */
        void addSelfEdges();

        std::vector<std::vector<std::unordered_set<ull>>>& getGraph() { return graph_->getInHashset(); }

        ull getEdgeCount() override;
    };
}