#pragma once

#include "utils/graphs/Graph.hpp"
#include "utils/graphs/Graph3DIn.hpp"
#include "utils/Edges.hpp"
#include "utils/Types.hpp"
#include "utils/Config.hpp"
#include "utils/Grammar.hpp"
#include "solvers/SolverBase.hpp"

namespace gracfl 
{
    /**
     * @class SolverBWGram
     * @brief Backward grammar-driven CFL solver.
     *
     * This solver performs context-free language reachability analysis using a backward
     * traversal strategy. It is driven by production rules in the grammar and operates on a
     * graph structure that supports incoming edge access (Graph3DIn).
     */
    class SolverBWGram : public SolverBase 
    {
    protected:
        Grammar& grammar_;
        Graph3DIn* graph_;
    public:
       
        SolverBWGram(std::string graphfilepath, Grammar& grammar);
        ~SolverBWGram();

        void runCFL() override;

        /**
         * @brief Performs one iteration of edge derivation.
         * 
         * In this iteration, derivations are performed based on the grammar rules
         * and the current state of the graph.
         * 
         * @param inEdges Incoming edge lists.
         * @param hashset Edge presence tracker (node × label → set of neighbors).
         * @param grammar2index Unary production rules.
         * @param grammar3indexRight Binary productions (right-side association).
         * @param labelSize Number of total symbols.
         * @param nodeSize Total number of nodes/vertex in the graph.
         * @param terminate Flag indicating whether convergence has been reached.
         */
        void runSingleIteration(
            std::vector<std::vector<TemporalVector>>& inEdges,
            std::vector<std::vector<std::unordered_set<ull>>>& hashset,
            std::vector<std::vector<uint>>& grammar2index,
            std::vector<std::vector<std::pair<uint, uint>>>& grammar3indexRight,
            uint labelSize,
            uint nodeSize,
            bool& terminate
        );

         /**
         * @brief Adds self-loop epsilon edges to support epsilon productions.
         */
        void addSelfEdges();


        std::vector<std::vector<std::unordered_set<ull>>> getGraph() override { 
            std::vector<std::vector<std::unordered_set<ull>>> outHashset = convertInHashsetToOutHashset(graph_->getInHashset());
            return outHashset; 
        }

        /**
         * @brief Retrieves the total number of CFL-reachable edges in the graph.
         * @return Number of reachable edges.
         */
        ull getEdgeCount() override;
    };
}