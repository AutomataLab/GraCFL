#include <fstream>
#include <string>
#include "utils/Grammar.hpp"
#include "utils/graphs/Graph.hpp"

namespace gracfl {
    Graph::Graph(std::string& graphfilepath, const Grammar& grammar)
    {
        numLabels_ = grammar.getLabelSize();
        loadGraphFile(graphfilepath, grammar);
    }

    void Graph::loadGraphFile(std::string& graphfilepath, const Grammar& grammar)
    {
        Edge newEdge;
        uint from, to;
        std::string label;

        std::ifstream infile(graphfilepath); 
        while (infile >> newEdge.from)
        {
            infile >> newEdge.to;
            infile >> label;

            if (grammar.getSymbolToIDMap().find(label) == grammar.getSymbolToIDMap().end())
            {
                continue;
            }

            newEdge.label = grammar.getSymbolToIDMap().at(label);
            edges_.push_back(newEdge);
            numNodes_ = std::max(numNodes_, std::max(newEdge.from + 1, newEdge.to + 1));
            numEdges_++;
        }
        infile.close();
    }

    ull Graph::countEdgeHelper(std::vector<std::vector<std::unordered_set<ull>>>& hashset)
    {
        ull size = 0;
        for (uint i = 0; i < hashset.size(); i++)
        {
            for (uint j = 0; j < hashset[i].size(); j++)
            {
                size += hashset[i][j].size();
            }
        }
        return size;
    }
}