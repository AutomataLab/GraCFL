#pragma once

#include <vector>
#include <unordered_set>
#include "../Edges.hpp"
#include "Graph.hpp"
#include "../Types.hpp"

namespace gracfl {
    /**
     * @class Graph3DIn
     * @brief A label-partitioned graph structure that stores only incoming edges for CFL reachability analysis.
     *
     * This class is designed to support backward traversal in context-free language (CFL) analysis.
     * Edges are organized by label and destination node, allowing for efficient grammar-driven evaluation.
     */
    class Graph3DIn : public Graph
    {
    public:
        std::vector<std::vector<TemporalVector>> inEdges_;
        std::vector<std::vector<std::unordered_set<ull>>> inHashset_;

        Graph3DIn(std::string& graphfilepath, const Grammar& grammar);
        void initContainers();
        void addInitialEdges();
        void clearContainers();
        void addSelfEdge(Edge& edge);
        void checkAndAddEdge(Edge& edge, bool& terminate);
        ull countEdge();

        inline std::vector<std::vector<TemporalVector>>& getInEdges() { return inEdges_; }
        inline std::vector<std::vector<std::unordered_set<ull>>>& getInHashset() { return inHashset_; }
    };
}
