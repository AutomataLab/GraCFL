#pragma once

#include <vector>
#include <unordered_set>
#include "../Edges.hpp"
#include "Graph.hpp"
#include "../Types.hpp"

namespace gracfl {
    /**
     * @class Graph2DIn
     * @brief A graph structure that stores only incoming edges for CFL reachability analysis.
     *
     * This class represents a directed graph that maintains only incoming edges and supports 
     * operations such as edge addition, presence checking, and edge counting. It is optimized 
     * for topological based backward traversal in context-free language reachability.
     */
    class Graph2DIn : public Graph
    {
    public:
        std::vector<TemporalVectorWithLbldVtx> inEdges_;
        std::vector<std::vector<std::unordered_set<ull>>> inHashset_;
        
        Graph2DIn(std::string& graphfilepath, const Grammar& grammar);
        void initContainers();
        void addInitialEdges();
        void clearContainers();
        void addSelfEdge(Edge& edge);
        void checkAndAddEdge(Edge& edge, bool& terminate);
        ull countEdge();

        inline std::vector<TemporalVectorWithLbldVtx>& getInEdges() { return inEdges_; }
        inline std::vector<std::vector<std::unordered_set<ull>>>& getInHashset() { return inHashset_; }
    };
}
