#include <iostream>
#include "solvers/SolverBWGram.hpp"

namespace gracfl 
{
    // Definition of class SolverBWGram
    SolverBWGram::SolverBWGram(Config& config, const Grammar& grammar)
    : Graph(config.graphfile_, grammar)
    {
        config_ = config;
        inEdges_.assign(grammar.getLabelSize(), std::vector<BufferEdge>(getNodeSize()));
        hashset_.assign(getNodeSize(), std::vector<std::unordered_set<ull>>(grammar.getLabelSize(), std::unordered_set<ull>()));
        addInitialEdges();
    }

    void  SolverBWGram::solve(const Grammar& grammar)
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

    void  SolverBWGram::singleIteration(const Grammar& grammar, bool& terminate)
    {
        for (uint g = 0; g < grammar.getLabelSize(); g++)
        {
            for (int i = 0; i < getNodeSize(); i++)
            {
                uint inNbr1;
                uint START_NEW = inEdges_[g][i].OLD_END;
                uint END_NEW = inEdges_[g][i].NEW_END;

                for (uint j = START_NEW; j < END_NEW; j++)
                {
                    inNbr1 = inEdges_[g][i].vertexList[j];
                    
                    for (uint m = 0; m < grammar.rule2Index(g).size(); m++)
                    {
                        uint A = grammar.rule2Index(g)[m];
                        EdgeForReading newEdge(inNbr1, i, A);
                        addEdge(newEdge, terminate);
                    }

                    for (uint m = 0; m < grammar.rule3RightIndex(g).size(); m++)
                    {
                        uint B = grammar.rule3RightIndex(g)[m].first;
                        uint A = grammar.rule3RightIndex(g)[m].second;

                        uint START_OLD_OUT = 0;
                        uint END_NEW_OUT = inEdges_[B][inNbr1].NEW_END;
                        for (uint h = START_OLD_OUT; h < END_NEW_OUT; h++)
                        {
                            uint inNbr2 = inEdges_[B][inNbr1].vertexList[h];
                            EdgeForReading newEdge(inNbr2, i, A);
                            addEdge(newEdge, terminate);
                        }
                    }
                }

                uint START_OLD = 0;
                uint END_OLD = inEdges_[g][i].OLD_END;
                for (uint j = START_OLD; j < END_OLD; j++)
                {
                    inNbr1 = inEdges_[g][i].vertexList[j];

                    for (uint m = 0; m < grammar.rule3RightIndex(g).size(); m++)
                    {
                        // C = g
                        uint B = grammar.rule3RightIndex(g)[m].first;
                        uint A = grammar.rule3RightIndex(g)[m].second;

                        uint START_NEW_OUT = inEdges_[B][inNbr1].OLD_END;
                        uint END_NEW_OUT = inEdges_[B][inNbr1].NEW_END;

                        for (uint h = START_NEW_OUT; h < END_NEW_OUT; h++)
                        {
                            uint inNbr2 = inEdges_[B][inNbr1].vertexList[h];
                            EdgeForReading newEdge(inNbr2, i, A);
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
                inEdges_[g][i].OLD_END = inEdges_[g][i].NEW_END;
                inEdges_[g][i].NEW_END = inEdges_[g][i].vertexList.size();
            }
        }
    }

    void SolverBWGram::addInitialEdges()
    {
        for (EdgeForReading edge : getEdges())
        {
            hashset_[edge.to][edge.label].insert(edge.from);
            inEdges_[edge.label][edge.to].vertexList.push_back(edge.from);
            // update the sliding pointers
            inEdges_[edge.label][edge.to].NEW_END++;
        }
    }

    void SolverBWGram::addAllSelfEdges(const Grammar& grammar)
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

    void SolverBWGram::addSelfEdge(EdgeForReading& edge)
    {
        if (hashset_[edge.to][edge.label].find(edge.from) == hashset_[edge.to][edge.label].end())
        {
            hashset_[edge.to][edge.label].insert(edge.from);
            inEdges_[edge.label][edge.to].vertexList.push_back(edge.from);
            inEdges_[edge.label][edge.to].NEW_END++;
        }
    }

    void SolverBWGram::addEdge(EdgeForReading& edge, bool& terminate) 
    {
        if (hashset_[edge.to][edge.label].find(edge.from) == hashset_[edge.to][edge.label].end()) {
            hashset_[edge.to][edge.label].insert(edge.from);
            inEdges_[edge.label][edge.to].vertexList.push_back(edge.from);
            terminate = false;
        }
    }

    ull SolverBWGram::countEdge()
    {
        return countEdgeHelper(hashset_);
    }
}