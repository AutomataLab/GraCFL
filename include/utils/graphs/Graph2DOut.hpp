#pragma once

#include <vector>
#include <unordered_set>
#include "../Edges.hpp"
#include "Graph.hpp"
#include "../Types.hpp"

namespace gracfl {
    /**
     * @class Graph2DOut
     * @brief A graph structure that stores only outgoing edges for CFL reachability analysis.
     *
     * This class maintains a directed graph where only outgoing edges are stored, along with 
     * a hashset to track edge presence efficiently. It supports operations to add edges, 
     * initialize containers, and compute edge statistics, useful in forward traversal of CFL solvers.
     */
    class Graph2DOut : public Graph
    {
    public:
        std::vector<TemporalVectorWithLbldVtx> outEdges_;
        std::vector<std::vector<std::unordered_set<ull>>> hashset_;
        
        Graph2DOut(std::string& graphfilepath, const Grammar& grammar);
        void initContainers();
        void addInitialEdges();
        void clearContainers();
        void addSelfEdge(Edge& edge);
        void checkAndAddEdge(Edge& edge, bool& terminate);
        ull countEdge();

        inline std::vector<TemporalVectorWithLbldVtx>& getOutEdges()  { return outEdges_; }
        inline std::vector<std::vector<std::unordered_set<ull>>>& getHashset() { return hashset_; }
    };
}
