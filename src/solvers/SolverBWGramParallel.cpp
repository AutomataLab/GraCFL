#include <iostream>
#include "solvers/SolverBWGramParallel.hpp"

namespace gracfl 
{
    SolverBWGramParallel::SolverBWGramParallel(std::string graphfilepath, Grammar& grammar, uint numOfThreads)
    : SolverBWGram(graphfilepath, grammar)
    {
        numOfThreads_ = numOfThreads;
    }

    void  SolverBWGramParallel::runCFL()
    { 
        uint itr = 0;
        bool terminate;
        auto& inEdges = graph_->inEdges_;
        auto& hashset = graph_->inHashset_;
        auto& grammar2index = grammar_.grammar2index_;
        auto& grammar3indexRight  = grammar_.grammar3indexRight_;
        auto labelSize = grammar_.getLabelSize();
        auto nodeSize = graph_->getNodeSize();

        addSelfEdges(); // add epsilon edges

        do {
            itr++;
            terminate = true;
            runSingleIterationParallel(
                inEdges,
                hashset, 
                grammar2index,
                grammar3indexRight,
                labelSize,
                nodeSize,
                terminate);
            std::cout << "Iteration " << itr << std::endl;
        } while(!terminate);
    }

    void SolverBWGramParallel::runSingleIterationParallel(
        std::vector<std::vector<TemporalVector>>& inEdges,
        std::vector<std::vector<std::unordered_set<ull>>>& inHashset,
        std::vector<std::vector<uint>>& grammar2index,
        std::vector<std::vector<std::pair<uint, uint>>>& grammar3indexRight,
        uint labelSize,
        uint nodeSize,
        bool& terminate)
    {
        for (uint g = 0; g < labelSize; g++)
        {
            #pragma omp parallel for schedule(static) num_threads(numOfThreads_)
            for (int i = 0; i < nodeSize; i++)
            {
                uint inNbr1;
                uint START_NEW = inEdges[g][i].OLD_END;
                uint END_NEW = inEdges[g][i].NEW_END;

                for (uint j = START_NEW; j < END_NEW; j++)
                {
                    inNbr1 = inEdges[g][i].vertexList[j];
                    
                    for (uint m = 0; m < grammar2index[g].size(); m++)
                    {
                        uint A = grammar2index[g][m];
                        Edge newEdge(inNbr1, i, A);
                        graph_->checkAndAddEdge(newEdge, terminate);
                    }

                    for (uint m = 0; m < grammar3indexRight[g].size(); m++)
                    {
                        uint B = grammar3indexRight[g][m].first;
                        uint A = grammar3indexRight[g][m].second;

                        uint START_OLD_OUT = 0;
                        uint END_NEW_OUT = inEdges[B][inNbr1].NEW_END;
                        for (uint h = START_OLD_OUT; h < END_NEW_OUT; h++)
                        {
                            uint inNbr2 = inEdges[B][inNbr1].vertexList[h];
                            Edge newEdge(inNbr2, i, A);
                            graph_->checkAndAddEdge(newEdge, terminate);
                        }
                    }
                }

                uint START_OLD = 0;
                uint END_OLD = inEdges[g][i].OLD_END;
                for (uint j = START_OLD; j < END_OLD; j++)
                {
                    inNbr1 = inEdges[g][i].vertexList[j];

                    for (uint m = 0; m < grammar3indexRight[g].size(); m++)
                    {
                        // C = g
                        uint B = grammar3indexRight[g][m].first;
                        uint A = grammar3indexRight[g][m].second;

                        uint START_NEW_OUT = inEdges[B][inNbr1].OLD_END;
                        uint END_NEW_OUT = inEdges[B][inNbr1].NEW_END;

                        for (uint h = START_NEW_OUT; h < END_NEW_OUT; h++)
                        {
                            uint inNbr2 = inEdges[B][inNbr1].vertexList[h];
                            Edge newEdge(inNbr2, i, A);
                            graph_->checkAndAddEdge(newEdge, terminate);
                        }
                    }
                }
            }
        }

        // ----------------- Update Sliding Pointers -----------------
        for (uint g = 0; g < labelSize; g++)
        {
            #pragma omp parallel for schedule(static) num_threads(numOfThreads_)
            for (int i = 0; i < nodeSize; i++)
            {
                inEdges[g][i].OLD_END = inEdges[g][i].NEW_END;
                inEdges[g][i].NEW_END = inEdges[g][i].vertexList.size();
            }
        }
    }
}