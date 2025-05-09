#pragma once

#include <vector>
#include <unordered_set>
#include "../Edges.hpp"
#include "Graph.hpp"
#include "../Types.hpp"

namespace gracfl {
    /**
     * @class Graph3DBi
     * @brief A 3D bidirectional graph structure for CFL reachability analysis.
     *
     * This class represents a graph where edges are grouped by both source/destination nodes and labels,
     * allowing label-sensitive traversal in both forward and backward directions. It is suited for
     * grammar-driven and label-partitioned CFL solvers.
     */
    class Graph3DBi : public Graph
    {
    public:
        std::vector<std::vector<TemporalVector>> outEdges_;
        std::vector<std::vector<TemporalVector>> inEdges_;
        std::vector<std::vector<std::unordered_set<ull>>> hashset_;
        
        Graph3DBi(std::string& graphfilepath, const Grammar& grammar);
        void initContainers();
        void addInitialEdges();
        void clearContainers();
        void addSelfEdge(Edge& edge);
        void checkAndAddEdge(Edge& edge, bool& terminate);
        ull countEdge();

        inline std::vector<std::vector<TemporalVector>>& getOutEdges()  { return outEdges_; }
        inline std::vector<std::vector<TemporalVector>>& getInEdges() { return inEdges_; }
        inline std::vector<std::vector<std::unordered_set<ull>>>& getHashset() { return hashset_; }
    };
}
