#pragma once

#include <vector>
#include <unordered_set>
#include "../Edges.hpp"
#include "Graph.hpp"
#include "../Types.hpp"

namespace gracfl {
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
