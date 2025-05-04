#include <iostream>
#include "solvers/SolverFWGram.hpp"

namespace gracfl 
{   
    SolverFWGram::SolverFWGram(Grammar& grammar, Graph3DOut& graph)
    : grammar_(grammar)
    , graph_  (graph)
    {
        // any extra setup can go here
    }

    void  SolverFWGram::runCFL()
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

    void  SolverFWGram::runSingleIteration(bool& terminate)
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

    void SolverFWGram::addSelfEdges()
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
}