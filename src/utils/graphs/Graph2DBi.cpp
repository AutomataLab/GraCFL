#include "utils/graphs/Graph2DBi.hpp"

namespace gracfl {
    Graph2DBi::Graph2DBi(std::string& graphfilepath, const Grammar& grammar)
        : Graph(graphfilepath, grammar) 
    {
        initContainers();
        addInitialEdges();
    }

    void Graph2DBi::initContainers()
    {
        outEdges_.assign(getNodeSize(), TemporalVectorWithLbldVtx());
        inEdges_.assign(getNodeSize(), TemporalVectorWithLbldVtx());
        hashset_.assign(getNodeSize(), std::vector<std::unordered_set<ull>>(getLabelSize(), std::unordered_set<ull>()));
    }

    void Graph2DBi::addInitialEdges()
    {
        for (Edge edge : getEdges())
        {
            hashset_[edge.from][edge.label].insert(edge.to);
            outEdges_[edge.from].vertexList.push_back(LbldVtx(edge.to, edge.label));
            inEdges_[edge.to].vertexList.push_back(LbldVtx(edge.from, edge.label));

            // update the sliding pointers
            outEdges_[edge.from].NEW_END++;
            inEdges_[edge.to].NEW_END++;
        }
    }

    void Graph2DBi::clearContainers()
    {
        outEdges_.clear();
        inEdges_.clear();
        hashset_.clear();
    }


    void Graph2DBi::checkAndAddEdge(Edge& edge, bool& terminate)
    {
        if (hashset_[edge.from][edge.label].find(edge.to) == hashset_[edge.from][edge.label].end()) {
            hashset_[edge.from][edge.label].insert(edge.to);
            outEdges_[edge.from].vertexList.push_back(LbldVtx(edge.to, edge.label));
            inEdges_[edge.to].vertexList.push_back(LbldVtx(edge.from, edge.label));
            terminate = false;
        }
    }

    void Graph2DBi::addSelfEdge(Edge& edge)
    {
        if (hashset_[edge.from][edge.label].find(edge.to) == hashset_[edge.from][edge.label].end())
        {
            hashset_[edge.from][edge.label].insert(edge.to);
            outEdges_[edge.from].vertexList.push_back(LbldVtx(edge.to, edge.label));
            outEdges_[edge.from].NEW_END++;
            inEdges_[edge.to].vertexList.push_back(LbldVtx(edge.from, edge.label));
            inEdges_[edge.to].NEW_END++;
        }
    }

    ull Graph2DBi::countEdge()
    {
        return countEdgeHelper(hashset_);
    }
}