#pragma once

#include "utils/Graph.hpp"
#include "utils/Edges.hpp"
#include "utils/Types.hpp"
#include "utils/Config.hpp"

namespace gracfl 
{
    /**
     * @class BIGracfl
     * @brief  Bidirectional CFL-reachability graph implementation and analysis using grammar-driven travesal and sliding pointers.
     * 
     * Inherits from Graph and adds support for bidirectional edge derivations.
     * Maintains separate out-edges and in-edges adjacency lists, as well as a hashset to avoid duplicates.
     */
    class BIGracfl : public Graph
    {
        /// Adjacency list for outgoing edges: [label][source_vtx].vertexList
        std::vector<std::vector<BufferEdge>> outEdges_;
        /// Adjacency list for incoming edges: [label][destination_vtx].vertexList
        std::vector<std::vector<BufferEdge>> inEdges_;
        /// Duplicate edge-check datastructure: [source][label] -> set of destinations
        std::vector<std::vector<std::unordered_set<ull>>> hashset_;
        /// Solver configuration parameters
        Config config_;
    public:
        /**
         * @brief Constructor allocates adjacency lists + hashset, and reads initial edges.
         * @param config Solver configurations.
         * @param grammar       Grammar describing the CFL rules.
         */
        BIGracfl(Config& config, const Grammar& grammar);

        /**
         * @brief Executes the full bidirectional CFL-reachability analysis.
         * @param grammar Grammar rules for generating new edges.
         */
        void solve(const Grammar& grammar) override;

        /**
         * @brief Performs one iteration of edge derivation.
         * @param grammar   Grammar rules for edge derivations.
         * @param terminate Flag set to false if new edges were added.
         */
        void singleIteration(const Grammar& grammar, bool& terminate);

        /**
         * @brief Initializes adjacency lists + hashsets with edges from the input graph.
         */
        void addInitialEdges();

        /**
         * @brief Adds self-loop edges (epsilon rules) for all nodes.
         * @param grammar Grammar rules for edge derivations.
         */
        void addAllSelfEdges(const Grammar& grammar);

        /**
         * @brief Adds a single self-edge if not already present.
         * @param edge Edge to insert (from, to, label).
         */
        void addSelfEdge(EdgeForReading& edge);

        /**
         * @brief Adds a general derived edge if not already present.
         * @param edge  Edge to insert (from, to, label).
         * @param terminate Flag reference, set false if insertion occurs.
         */
        void addEdge(EdgeForReading& edge, bool& terminate);

        /**
         * @brief Counts total number of distinct edges in the graph.
         * @returns Total edge count.
         */
        ull countEdge() override;

        /**
         * @brief Accessor for outgoing-edge adjacency list.
         * @returns Reference to outEdges_.
         */
        inline std::vector<std::vector<BufferEdge>>& getOutEdges() { return outEdges_; }

        /**
         * @brief Accessor for incoming-edge adjacency list.
         * @returns Reference to inEdges_.
         */
        inline std::vector<std::vector<BufferEdge>>& getInEdges() { return inEdges_; }

        /**
         * @brief Accessor for duplicate-edge-check hashsets.
         * @returns Reference to hashset_.
         */
        inline std::vector<std::vector<std::unordered_set<ull>>>& getHashset() { return hashset_; }
    };
}