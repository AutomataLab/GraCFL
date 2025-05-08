#pragma once

#include "utils/graphs/Graph.hpp"
#include "utils/graphs/Graph3DOut.hpp"
#include "utils/Edges.hpp"
#include "utils/Types.hpp"
#include "utils/Config.hpp"
#include "utils/Grammar.hpp"
#include "solvers/SolverBase.hpp"

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
    protected:
        Grammar& grammar_;
        Graph3DOut* graph_;
    public:
        SolverFWGram(std::string graphfilepath, Grammar& grammar);
        ~SolverFWGram();

        void runCFL() override;

        void runSingleIteration(
            std::vector<std::vector<TemporalVector>>& outEdges,
            std::vector<std::vector<std::unordered_set<ull>>>& hashset,
            std::vector<std::vector<uint>>& grammar2index,
            std::vector<std::vector<std::pair<uint, uint>>>& grammar3indexLeft,
            uint labelSize,
            uint nodeSize,
            bool& terminate);

        /**
         * @brief Adds self-loop edges (epsilon rules) for all nodes.
         * @param grammar Grammar rules for edge derivations.
         */
        void addSelfEdges();

        std::vector<std::vector<std::unordered_set<ull>>>& getGraph() { return graph_->getHashset(); }

        ull getEdgeCount() override;
    };
}