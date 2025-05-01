#include <iostream>
#include "gracfl/FWGracfl.hpp"

namespace gracfl 
{
    FWGracfl::FWGracfl(Config& config, const Grammar& grammar)
    : Graph(config.graphfile_, grammar)
    {
        config_ =  config;
        outEdges_.assign(grammar.getLabelSize(), std::vector<BufferEdge>(getNodeSize()));
        hashset_.assign(getNodeSize(), std::vector<std::unordered_set<ull>>(grammar.getLabelSize(), std::unordered_set<ull>()));
        addInitialEdges();
    }
    
    void  FWGracfl::solve(const Grammar& grammar)
    { 
        addAllSelfEdges(grammar); // add epsilon edges
        uint itr = 0;
        bool terminate;
        do {
            itr++;
            terminate = true;
            singleIteration(grammar, terminate);
            std::cout << "Iteration " << itr << std::endl;
        } while(!terminate);
    }

    void  FWGracfl::singleIteration(const Grammar& grammar, bool& terminate)
    {
        for (uint g = 0; g < grammar.getLabelSize(); g++)
        {
            for (uint i = 0; i < getNodeSize(); i++)
            {
                uint nbr;
                uint START_NEW = outEdges_[g][i].OLD_END;
                uint END_NEW = outEdges_[g][i].NEW_END;

                for (uint j = START_NEW; j < END_NEW; j++)
                {
                    nbr = outEdges_[g][i].vertexList[j];
                    for (uint m = 0; m < grammar.rule2Index(g).size(); m++)
                    {
                        uint A = grammar.rule2Index(g)[m];
                        EdgeForReading newEdge(i, nbr, A);
                        addEdge(newEdge, terminate);
                    }

                    for (uint m = 0; m < grammar.rule3LeftIndex(g).size(); m++)
                    {
                        uint C = grammar.rule3LeftIndex(g)[m].first;
                        uint A = grammar.rule3LeftIndex(g)[m].second;

                        uint START_OLD_OUT = 0;
                        uint END_NEW_OUT = outEdges_[C][nbr].NEW_END;
                        for (uint h = START_OLD_OUT; h < END_NEW_OUT; h++)
                        {
                            uint outNbr = outEdges_[C][nbr].vertexList[h];
                            EdgeForReading newEdge(i, outNbr, A);
                            addEdge(newEdge, terminate);
                        }
                    }
                }

                uint START_OLD = 0;
                uint END_OLD = outEdges_[g][i].OLD_END;
                for (uint j = START_OLD; j < END_OLD; j++)
                {
                    nbr = outEdges_[g][i].vertexList[j];
                    for (uint m = 0; m < grammar.rule3LeftIndex(g).size(); m++)
                    {
                        uint C = grammar.rule3LeftIndex(g)[m].first;
                        uint A = grammar.rule3LeftIndex(g)[m].second;

                        uint START_NEW_OUT = outEdges_[C][nbr].OLD_END;
                        uint END_NEW_OUT = outEdges_[C][nbr].NEW_END;
                        for (uint h = START_NEW_OUT; h < END_NEW_OUT; h++)
                        {
                            uint outNbr = outEdges_[C][nbr].vertexList[h];
                            EdgeForReading newEdge(i, outNbr, A);
                            addEdge(newEdge, terminate);
                        }
                    }
                }
            }
        }

        // ----------------- Update Sliding Pointers -----------------
        for (uint g = 0; g < grammar.getLabelSize(); g++)
        {
            for (int i = 0; i < getNodeSize(); i++)
            {
                outEdges_[g][i].OLD_END = outEdges_[g][i].NEW_END;
                outEdges_[g][i].NEW_END = outEdges_[g][i].vertexList.size();
            }
        }
    }

    void FWGracfl::addInitialEdges()
    {
        for (EdgeForReading edge : getEdges())
        {
            hashset_[edge.from][edge.label].insert(edge.to);
            outEdges_[edge.label][edge.from].vertexList.push_back(edge.to);
            
            // Update NEW_END pointers
            outEdges_[edge.label][edge.from].NEW_END++;
        }
    }

    void FWGracfl::addAllSelfEdges(const Grammar& grammar)
    {
        for (int l = 0; l < grammar.getRule1().size(); l++)
        {
            for (int i = 0; i < getNodeSize(); i++)
            {
                EdgeForReading edge(i, i, grammar.getRule1()[l][0]);
                addSelfEdge(edge);
            }
        }
    }

    void FWGracfl::addSelfEdge(EdgeForReading& edge)
    {
        // check if the new edge based on an epsilon grammar rule exists or not. l: grammar ID, 0: LHS
        if (hashset_[edge.from][edge.label].find(edge.to) == hashset_[edge.from][edge.label].end())
        {
            hashset_[edge.from][edge.label].insert(edge.to);
            outEdges_[edge.label][edge.from].vertexList.push_back(edge.to);
            outEdges_[edge.label][edge.from].NEW_END++;
        }
    }

    void FWGracfl::addEdge(EdgeForReading& edge, bool& terminate) 
    {
        if (hashset_[edge.from][edge.label].find(edge.to) == hashset_[edge.from][edge.label].end()) {
            hashset_[edge.from][edge.label].insert(edge.to);
            outEdges_[edge.label][edge.from].vertexList.push_back(edge.to);
            terminate = false;
        }
    }

    ull FWGracfl::countEdge()
    {
        return countEdgeHelper(hashset_);
    }
}