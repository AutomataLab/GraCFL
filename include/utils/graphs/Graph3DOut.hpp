#pragma once

#include <vector>
#include <unordered_set>
#include "../Edges.hpp"
#include "Graph.hpp"
#include "../Types.hpp"

namespace gracfl {
    /**
     * @class Graph3DOut
     * @brief A 3D graph structure that stores only outgoing edges for CFL reachability analysis.
     *
     * This class organizes outgoing edges by both labels and source nodes, enabling label-partitioned
     * graph traversal. It is optimized for forward grammar-driven CFL solving, allowing fast edge 
     * lookups and efficient memory usage for large graphs.
     */
    class Graph3DOut : public Graph
    {
    public:
        std::vector<std::vector<TemporalVector>> outEdges_;
        std::vector<std::vector<std::unordered_set<ull>>> hashset_;
        
        Graph3DOut(std::string& graphfilepath, const Grammar& grammar);
        void initContainers();
        void addInitialEdges();
        void clearContainers();
        void addSelfEdge(Edge& edge);
        void checkAndAddEdge(Edge& edge, bool& terminate);
        ull countEdge();

        inline std::vector<std::vector<TemporalVector>>& getOutEdges() { return outEdges_; }
        inline std::vector<std::vector<std::unordered_set<ull>>>& getHashset() { return hashset_; }
    };
}
