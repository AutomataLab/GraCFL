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
     * @brief Backward topological CFL solver using 2D incoming edge graph.
     *
     * This solver applies topological processing strategies to perform context-free language
     * reachability analysis in the backward direction, operating on a Graph2DIn structure.
     */
    class SolverBWTopo : public SolverBase
    {
    protected:
        Grammar& grammar_;  ///< Reference to the grammar containing CFL production rules.
        Graph2DIn* graph_;  ///< Pointer to the 2D incoming edge graph representation.
    public:
        /**
         * @brief Constructor for SolverBWTopo.
         * @param graphfilepath Path to the input graph file.
         * @param grammar Reference to the Grammar object for rule processing.
         */
        SolverBWTopo(std::string graphfilepath, Grammar& grammar);

        /**
         * @brief Destructor.
         */
        ~SolverBWTopo();

        /**
         * @brief Executes the main CFL solving loop until convergence is reached.
         */
        void runCFL() override;

        /**
         * @brief Executes one iteration of the CFL solving process using topological rule evaluation.
         *
         * @param inEdges Incoming labeled neighbor vectors (used for traversal).
         * @param inHashset Reachability set in backward form (label × node → {source nodes}).
         * @param grammar2index unary grammar rules.
         * @param grammar3index binary grammar rules.
         * @param labelSize Total number of symbols.
         * @param nodeSize Total number of nodes in the graph.
         * @param terminate Flag indicating whether convergence has been reached.
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