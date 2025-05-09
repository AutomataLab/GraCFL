#pragma once

#include <vector>
#include <unordered_set>
#include "../Edges.hpp"
#include "Graph.hpp"
#include "../Types.hpp"

namespace gracfl {
    /**
     * @class Graph2DBi
     * @brief A bidirectional graph structure for context-free language (CFL) reachability analysis.
     *
     * This class implements a 2D bidirectional graph where both incoming and outgoing edges are stored 
     * separately with associated labels. It provides mechanisms for initializing containers, adding edges, 
     * and tracking edge presence using hash sets. It is particularly suited for CFL-reachability solvers.
     */
    class Graph2DBi : public Graph
    {
    public:
        std::vector<TemporalVectorWithLbldVtx> outEdges_; ///< Outgoing edges organized by  source vertex
        std::vector<TemporalVectorWithLbldVtx> inEdges_; ///< Incoming edges organized by  destination vertex
        std::vector<std::vector<std::unordered_set<ull>>> hashset_; ///< Hashset for edge presence tracking (node × label → set of out neighbors)
        
        /**
         * @brief Constructs a Graph2DBi object.
         * @param graphfilepath Path to the graph file to be loaded.
         * @param grammar Reference to the grammar object for labeled edge interpretation.
         */
        Graph2DBi(std::string& graphfilepath, const Grammar& grammar);

        /**
         * @brief Initializes internal containers for graph data.
         */
        void initContainers();

        /**
         * @brief  adds the initial edges to the containers.
         */
        void addInitialEdges();

        /**
         * @brief Clears the internal data containers (edges and hashset).
         */
        void clearContainers();

        /**
         * @brief Adds a self-loop edge to the graph.
         * @param edge The edge to be added as a self-loop.
         */
        void addSelfEdge(Edge& edge);

        /**
         * @brief Conditionally adds an edge if it does not already exist.
         * @param edge The edge to check and potentially add.
         * @param terminate Reference to a boolean used to track termination condition.
         */
        void checkAndAddEdge(Edge& edge, bool& terminate);

        /**
         * @brief Counts the total number of edges in the graph.
         * @return The total number of edges.
         */
        ull countEdge();

        /**
         * @brief Retrieves the outgoing edges of the graph.
         * @return Reference to the vector of outgoing edge containers.
         */
        inline std::vector<TemporalVectorWithLbldVtx>& getOutEdges()  { return outEdges_; }

        /**
         * @brief Retrieves the incoming edges of the graph.
         * @return Reference to the vector of incoming edge containers.
         */
        inline std::vector<TemporalVectorWithLbldVtx>& getInEdges() { return inEdges_; }

        /**
         * @brief Retrieves the edge presence hashset.
         * @return Reference to the 3D hashset used to track edges.
         */
        inline std::vector<std::vector<std::unordered_set<ull>>>& getHashset() { return hashset_; }
    };
}
