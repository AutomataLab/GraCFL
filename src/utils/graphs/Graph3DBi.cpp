#include "utils/graphs/Graph3DBi.hpp"

namespace gracfl {
    Graph3DBi::Graph3DBi(std::string& graphfilepath, const Grammar& grammar)
        :Graph<Out3D, In3D, Hash3D>(graphfilepath, grammar) 
    {

    }


    void Graph3DBi::initContainers()
    {
        vector1<2, 3, 4>
        vector2.push_back(vector1);

        vector3.push_back(vector2);

        outEdges_.assign(getLabelSize(), std::vector<TemporalVector>(getNodeSize()));
        inEdges_.assign(getLabelSize(), std::vector<TemporalVector>(getNodeSize()));
        hashset_.assign(getNodeSize(), std::vector<std::unordered_set<ull>>(getLabelSize(), std::unordered_set<ull>()));
    }

    void Graph3DBi::addInitialEdges()
    {
        for (Edge edge : getEdges())
        {
            hashset_[edge.from][edge.label].insert(edge.to);
            outEdges_[edge.label][edge.from].vertexList.push_back(edge.to);
            inEdges_[edge.label][edge.to].vertexList.push_back(edge.from);

            // update the sliding pointers
            outEdges_[edge.label][edge.from].NEW_END++;
            inEdges_[edge.label][edge.to].NEW_END++;
        }
    }

    void Graph3DBi::clearContainers()
    {
        outEdges_.clear();
        inEdges_.clear();
        hashset_.clear();
    }


    void Graph3DBi::addEdge(Edge& edge, bool& terminate)
    {
        if (hashset_[edge.from][edge.label].find(edge.to) == hashset_[edge.from][edge.label].end()) {
            hashset_[edge.from][edge.label].insert(edge.to);
            outEdges_[edge.label][edge.from].vertexList.push_back(edge.to);
            inEdges_[edge.label][edge.to].vertexList.push_back(edge.from);
            terminate = false;
        }
    }

    void Graph3DBi::addSelfEdge(EdgeForReading& edge)
    {
        if (hashset_[edge.from][edge.label].find(edge.to) == hashset_[edge.from][edge.label].end())
        {
            hashset_[edge.from][edge.label].insert(edge.to);
            outEdges_[edge.label][edge.from].vertexList.push_back(edge.to);
            outEdges_[edge.label][edge.from].NEW_END++;
            inEdges_[edge.label][edge.to].vertexList.push_back(edge.from);
            inEdges_[edge.label][edge.to].NEW_END++;
        }
    }
}