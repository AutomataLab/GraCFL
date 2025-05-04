#include "utils/graphs/Graph3DOut.hpp"

namespace gracfl {
    Graph3DOut::Graph3DOut(std::string& graphfilepath, const Grammar& grammar)
        :Graph<Out3D, In3D, Hash3D>(graphfilepath, grammar) 
    {

    }
    
    void Graph3DOut::initContainers()
    {
        outEdges_.assign(getLabelSize(), std::vector<TemporalVector>(getNodeSize()));
        hashset_.assign(getNodeSize(), std::vector<std::unordered_set<ull>>(getLabelSize(), std::unordered_set<ull>()));
    }

    void Graph3DOut::addInitialEdges()
    {
        for (Edge edge : getEdges())
        {
            hashset_[edge.from][edge.label].insert(edge.to);
            outEdges_[edge.label][edge.from].vertexList.push_back(edge.to);

            // update the sliding pointers
            outEdges_[edge.label][edge.from].NEW_END++;
        }
    }

    void Graph3DOut::clearContainers()
    {
        outEdges_.clear();
        hashset_.clear();
    }

    void Graph3DOut::addEdge(Edge& edge, bool& terminate)
    {
        if (hashset_[edge.from][edge.label].find(edge.to) == hashset_[edge.from][edge.label].end()) {
            hashset_[edge.from][edge.label].insert(edge.to);
            outEdges_[edge.label][edge.from].vertexList.push_back(edge.to);
            terminate = false;
        }
    }

    void Graph3DOut::addSelfEdge(Edge& edge)
    {
        if (hashset_[edge.from][edge.label].find(edge.to) == hashset_[edge.from][edge.label].end())
        {
            hashset_[edge.from][edge.label].insert(edge.to);
            outEdges_[edge.label][edge.from].vertexList.push_back(edge.to);
            outEdges_[edge.label][edge.from].NEW_END++;
        }
    }
    
}