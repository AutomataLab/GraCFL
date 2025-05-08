#include "utils/graphs/Graph2DOut.hpp"

namespace gracfl {
    Graph2DOut::Graph2DOut(std::string& graphfilepath, const Grammar& grammar)
        : Graph(graphfilepath, grammar) 
    {
        initContainers();
        addInitialEdges();
    }

    void Graph2DOut::initContainers()
    {
        outEdges_.assign(getNodeSize(), TemporalVectorWithLbldVtx());
        hashset_.assign(getNodeSize(), std::vector<std::unordered_set<ull>>(getLabelSize(), std::unordered_set<ull>()));
    }

    void Graph2DOut::addInitialEdges()
    {
        for (Edge edge : getEdges())
        {
            hashset_[edge.from][edge.label].insert(edge.to);
            outEdges_[edge.from].vertexList.push_back(LbldVtx(edge.to, edge.label));

            // update the sliding pointers
            outEdges_[edge.from].NEW_END++;
        }
    }

    void Graph2DOut::clearContainers()
    {
        outEdges_.clear();
        hashset_.clear();
    }


    void Graph2DOut::checkAndAddEdge(Edge& edge, bool& terminate)
    {
        if (hashset_[edge.from][edge.label].find(edge.to) == hashset_[edge.from][edge.label].end()) {
            hashset_[edge.from][edge.label].insert(edge.to);
            outEdges_[edge.from].vertexList.push_back(LbldVtx(edge.to, edge.label));
            terminate = false;
        }
    }

    void Graph2DOut::addSelfEdge(Edge& edge)
    {
        if (hashset_[edge.from][edge.label].find(edge.to) == hashset_[edge.from][edge.label].end())
        {
            hashset_[edge.from][edge.label].insert(edge.to);
            outEdges_[edge.from].vertexList.push_back(LbldVtx(edge.to, edge.label));
            outEdges_[edge.from].NEW_END++;
        }
    }

    ull Graph2DOut::countEdge()
    {
        return countEdgeHelper(hashset_);
    }
}