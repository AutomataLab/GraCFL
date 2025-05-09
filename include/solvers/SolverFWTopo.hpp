#pragma once

#include "utils/graphs/Graph.hpp"
#include "utils/graphs/Graph2DOut.hpp"
#include "utils/Edges.hpp"
#include "utils/Types.hpp"
#include "utils/Config.hpp"
#include "utils/Grammar.hpp"
#include "solvers/SolverBase.hpp"

namespace gracfl 
{
    /**
     * @class SolverFWTopo
     * @brief Forward topology-driven solver for CFL reachability analysis.
     *
     * This solver uses a topological strategy to process edges over a 2D forward graph (Graph2DOut).
     * It is efficient for grammars where topological traversal of labeled nodes improves convergence speed.
     */
    class SolverFWTopo : public SolverBase
    {
    protected:
        Grammar& grammar_;  ///< Reference to the grammar used for parsing and derivation.
        Graph2DOut* graph_; ///< Pointer to the forward 2D graph structure.
    public:
        /**
         * @brief Constructor for SolverFWTopo.
         * @param graphfilepath Path to the input graph file.
         * @param grammar Reference to the Grammar object used for CFL derivations.
         */
        SolverFWTopo(std::string graphfilepath, Grammar& grammar);
        /**
         * @brief Destructor.
         */
        ~SolverFWTopo();

        /**
         * @brief Executes the CFL solver loop until convergence is achieved.
         */
        void runCFL() override;

        /**
         * @brief Runs a single iteration of the CFL solving process.
         *
         * Applies production rules using outgoing edges to perform updates
         * to the graph via hashsets.
         *
         * @param outEdges Outgoing edge lists.
         * @param hashset Edge presence tracker (label × node → set of neighbors).
         * @param grammar2index Unary production rules.
         * @param grammar3index Binary productions (left-side association).
         * @param labelSize Number of total symbols.
         * @param nodeSize Total number of nodes/vertex in the graph.
         * @param terminate Flag indicating whether convergence has been reached.
         */
        void runSingleIteration(
            std::vector<TemporalVectorWithLbldVtx>& outEdges,
            std::vector<std::vector<std::unordered_set<ull>>>& hashset,
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

        std::vector<std::vector<std::unordered_set<ull>>> getGraph() override { return graph_->getHashset(); }

        /**
         * @brief Retrieves the total number of CFL-reachable edges in the graph.
         * @return Number of reachable edges.
         */
        ull getEdgeCount() override;
    };
}