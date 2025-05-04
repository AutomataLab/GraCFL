#include "utils/graphs/Graph3DIn.hpp"

namespace gracfl {
    Graph3DIn::Graph3DIn(std::string& graphfilepath, const Grammar& grammar)
        :Graph<Out3D, In3D, Hash3D>(graphfilepath, grammar) 
    {

    }

    void Graph3DIn::initContainers()
    {
        inEdges_.assign(getLabelSize(), std::vector<TemporalVector>(getNodeSize()));
        hashset_.assign(getNodeSize(), std::vector<std::unordered_set<ull>>(getLabelSize(), std::unordered_set<ull>()));
    }

    void Graph3DIn::addInitialEdges()
    {
        for (Edge edge : getEdges())
        {
            hashset_[edge.from][edge.label].insert(edge.to);
            inEdges_[edge.label][edge.to].vertexList.push_back(edge.from);

            // update the sliding pointers
            inEdges_[edge.label][edge.to].NEW_END++;
        }
    }

    void Graph3DIn::clearContainers()
    {
        inEdges_.clear();
        hashset_.clear();
    }

    void Graph3DIn::addEdge(Edge& edge, bool& terminate)
    {
        if (hashset_[edge.from][edge.label].find(edge.to) == hashset_[edge.from][edge.label].end()) {
            hashset_[edge.from][edge.label].insert(edge.to);
            inEdges_[edge.label][edge.to].vertexList.push_back(edge.from);
            terminate = false;
        }
    }
    
    void Graph3DIn::addSelfEdge(Edge& edge)
    {
        if (hashset_[edge.from][edge.label].find(edge.to) == hashset_[edge.from][edge.label].end())
        {
            hashset_[edge.from][edge.label].insert(edge.to);
            inEdges_[edge.label][edge.to].vertexList.push_back(edge.from);
            inEdges_[edge.label][edge.to].NEW_END++;
        }
    }
}