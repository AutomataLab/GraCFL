#include <iostream>
#include "solvers/SolverBIGram.hpp"

namespace gracfl 
{

    SolverBIGram::SolverBIGram(Config& config, const Grammar& grammar)
    : Graph(config.graphfile_, grammar)
    {
        config_ = config;
        outEdges_.assign(grammar.getLabelSize(), std::vector<BufferEdge>(getNodeSize()));
        inEdges_.assign(grammar.getLabelSize(), std::vector<BufferEdge>(getNodeSize()));
        hashset_.assign(getNodeSize(), std::vector<std::unordered_set<ull>>(grammar.getLabelSize(), std::unordered_set<ull>()));
        addInitialEdges();
    }

    void  SolverBIGram::solve(const Grammar& grammar)
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

    void  SolverBIGram::singleIteration(const Grammar& grammar, bool& terminate)
    {
        // Derive new edges based on grammar rules
        for (uint g = 0; g < grammar.getLabelSize(); g++)
        {
            for (uint i = 0; i < getNodeSize(); i++)
            {
                uint nbr;
                uint START_NEW = inEdges_[g][i].OLD_END;
                uint END_NEW = inEdges_[g][i].NEW_END;
                // Process new in-edges labeled g
                for (uint j = START_NEW; j < END_NEW; j++)
                {
                    uint inNbr = inEdges_[g][i].vertexList[j];

                    // ------- Rule Type: A = B -------
                    for (uint m = 0; m < grammar.rule2Index(g).size(); m++)
                    {
                        uint A = grammar.rule2Index(g)[m];
                        EdgeForReading newEdge(inNbr, i, A);
                        addEdge(newEdge, terminate);
                    }

                    // ------- Rule Type: A = BC -------
                    for (uint m = 0; m < grammar.rule3LeftIndex(g).size(); m++)
                    {
                        uint C = grammar.rule3LeftIndex(g)[m].first;
                        uint A = grammar.rule3LeftIndex(g)[m].second;

                        uint START_OLD_OUT = 0;
                        uint END_NEW_OUT = outEdges_[C][i].NEW_END;
                        for (uint h = START_OLD_OUT; h < END_NEW_OUT; h++)
                        {
                            uint nbr = outEdges_[C][i].vertexList[h];
                            EdgeForReading newEdge(inNbr, nbr, A);
                            addEdge(newEdge, terminate);
                        }
                    }
                }


                uint START_NEW_OUT = outEdges_[g][i].OLD_END;
                uint END_NEW_OUT = outEdges_[g][i].NEW_END;
                // Process new out-edges labeled g
                for (uint j = START_NEW_OUT; j < END_NEW_OUT; j++)
                {   
                    uint nbr = outEdges_[g][i].vertexList[j];

                    // ------- Rule Type 3: A = CB -------
                    for (uint m = 0; m < grammar.rule3RightIndex(g).size(); m++)
                    {
                        uint C = grammar.rule3RightIndex(g)[m].first;
                        uint A = grammar.rule3RightIndex(g)[m].second;

                        uint START_OLD_IN = 0;
                        uint END_OLD_IN = inEdges_[C][i].OLD_END;
                        for (uint h = START_OLD_IN; h < END_OLD_IN; h++)
                        {
                            uint inNbr = inEdges_[C][i].vertexList[h];
                            EdgeForReading newEdge(inNbr, nbr, A);
                            addEdge(newEdge, terminate);
                        }
                    }
                }
            }
        }

        // Update sliding pointers for next iteration
        for (uint g = 0; g < grammar.getLabelSize(); g++)
        {
            for (int i = 0; i < getNodeSize(); i++)
            {
                outEdges_[g][i].OLD_END = outEdges_[g][i].NEW_END;
                outEdges_[g][i].NEW_END = outEdges_[g][i].vertexList.size();
                inEdges_[g][i].OLD_END = inEdges_[g][i].NEW_END; 
                inEdges_[g][i].NEW_END = inEdges_[g][i].vertexList.size();
            }
        }
    }

    void SolverBIGram::addInitialEdges()
    {
        for (EdgeForReading edge : getEdges())
        {
            hashset_[edge.from][edge.label].insert(edge.to);
            outEdges_[edge.label][edge.from].vertexList.push_back(edge.to);
            inEdges_[edge.label][edge.to].vertexList.push_back(edge.from);

            // update the sliding pointers
            outEdges_[edge.label][edge.from].NEW_END++;
            inEdges_[edge.label][edge.to].NEW_END++;
        }
    }

    void SolverBIGram::addAllSelfEdges(const Grammar& grammar)
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

    void SolverBIGram::addSelfEdge(EdgeForReading& edge)
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

    void SolverBIGram::addEdge(EdgeForReading& edge, bool& terminate)
    {
        if (hashset_[edge.from][edge.label].find(edge.to) == hashset_[edge.from][edge.label].end()) {
            hashset_[edge.from][edge.label].insert(edge.to);
            outEdges_[edge.label][edge.from].vertexList.push_back(edge.to);
            inEdges_[edge.label][edge.to].vertexList.push_back(edge.from);
            terminate = false;
        }
    }

    ull SolverBIGram::countEdge()
    {
        return countEdgeHelper(hashset_);
    }
}