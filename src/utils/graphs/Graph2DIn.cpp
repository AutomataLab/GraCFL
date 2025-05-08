#include "utils/graphs/Graph2DIn.hpp"

namespace gracfl {
    Graph2DIn::Graph2DIn(std::string& graphfilepath, const Grammar& grammar)
        : Graph(graphfilepath, grammar) 
    {
        initContainers();
        addInitialEdges();
    }

    void Graph2DIn::initContainers()
    {
        inEdges_.assign(getNodeSize(), TemporalVectorWithLbldVtx());
        inHashset_.assign(getNodeSize(), std::vector<std::unordered_set<ull>>(getLabelSize(), std::unordered_set<ull>()));
    }

    void Graph2DIn::addInitialEdges()
    {
        for (Edge edge : getEdges())
        {
            inHashset_[edge.to][edge.label].insert(edge.from);
            inEdges_[edge.to].vertexList.push_back(LbldVtx(edge.from, edge.label));

            // update the sliding pointers
            inEdges_[edge.to].NEW_END++;
        }
    }

    void Graph2DIn::clearContainers()
    {
        inEdges_.clear();
        inHashset_.clear();
    }


    void Graph2DIn::checkAndAddEdge(Edge& edge, bool& terminate)
    {
        if (inHashset_[edge.to][edge.label].find(edge.from) == inHashset_[edge.to][edge.label].end()) {
            inHashset_[edge.to][edge.label].insert(edge.from);
            inEdges_[edge.to].vertexList.push_back(LbldVtx(edge.from, edge.label));
            terminate = false;
        }
    }

    void Graph2DIn::addSelfEdge(Edge& edge)
    {
        if (inHashset_[edge.to][edge.label].find(edge.from) == inHashset_[edge.to][edge.label].end())
        {
            inHashset_[edge.to][edge.label].insert(edge.from);
            inEdges_[edge.to].vertexList.push_back(LbldVtx(edge.from, edge.label));
            inEdges_[edge.to].NEW_END++;
        }
    }

    ull Graph2DIn::countEdge()
    {
        return countEdgeHelper(inHashset_);
    }
}