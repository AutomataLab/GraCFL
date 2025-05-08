#include <iostream>
#include "solvers/SolverFWGramParallel.hpp"

namespace gracfl 
{
    SolverFWGramParallel::SolverFWGramParallel(std::string graphfilepath, Grammar& grammar, uint numOfThreads)
    : SolverFWGram(graphfilepath, grammar)
    {
        numOfThreads_ = numOfThreads;
    }

    void  SolverFWGramParallel::runCFL()
    { 
        uint itr = 0;
        bool terminate;
        auto& outEdges = graph_->outEdges_;
        auto& hashset = graph_->hashset_;
        auto& grammar2index = grammar_.grammar2index_;
        auto& grammar3indexLeft  = grammar_.grammar3indexLeft_;
        auto labelSize = grammar_.getLabelSize();
        auto nodeSize = graph_->getNodeSize();

        addSelfEdges(); // add epsilon edges

        do {
            itr++;
            terminate = true;
            runSingleIterationParallel(
                outEdges,
                hashset, 
                grammar2index,
                grammar3indexLeft,
                labelSize,
                nodeSize,
                terminate);
            std::cout << "Iteration " << itr << std::endl;
        } while(!terminate);
    }

    void SolverFWGramParallel::runSingleIterationParallel(
        std::vector<std::vector<TemporalVector>>& outEdges,
        std::vector<std::vector<std::unordered_set<ull>>>& hashset,
        std::vector<std::vector<uint>>& grammar2index,
        std::vector<std::vector<std::pair<uint, uint>>>& grammar3indexLeft,
        uint labelSize,
        uint nodeSize,
        bool& terminate)
    {
        for (uint g = 0; g < labelSize; g++)
        {
            #pragma omp parallel for schedule(static) num_threads(numOfThreads_)
            for (uint i = 0; i < nodeSize; i++)
            {
                uint nbr;
                uint START_NEW = outEdges[g][i].OLD_END;
                uint END_NEW = outEdges[g][i].NEW_END;

                for (uint j = START_NEW; j < END_NEW; j++)
                {
                    nbr = outEdges[g][i].vertexList[j];
                    for (uint m = 0; m < grammar2index[g].size(); m++)
                    {
                        uint A = grammar2index[g][m];
                        Edge newEdge(i, nbr, A);
                        graph_->checkAndAddEdge(newEdge, terminate);
                    }

                    for (uint m = 0; m < grammar3indexLeft[g].size(); m++)
                    {
                        uint C = grammar3indexLeft[g][m].first;
                        uint A = grammar3indexLeft[g][m].second;

                        uint START_OLD_OUT = 0;
                        uint END_NEW_OUT = outEdges[C][nbr].NEW_END;
                        for (uint h = START_OLD_OUT; h < END_NEW_OUT; h++)
                        {
                            uint outNbr = outEdges[C][nbr].vertexList[h];
                            Edge newEdge(i, outNbr, A);
                            graph_->checkAndAddEdge(newEdge, terminate);
                        }
                    }
                }

                uint START_OLD = 0;
                uint END_OLD = outEdges[g][i].OLD_END;
                for (uint j = START_OLD; j < END_OLD; j++)
                {
                    nbr = outEdges[g][i].vertexList[j];
                    for (uint m = 0; m < grammar3indexLeft[g].size(); m++)
                    {
                        uint C = grammar3indexLeft[g][m].first;
                        uint A = grammar3indexLeft[g][m].second;

                        uint START_NEW_OUT = outEdges[C][nbr].OLD_END;
                        uint END_NEW_OUT = outEdges[C][nbr].NEW_END;
                        for (uint h = START_NEW_OUT; h < END_NEW_OUT; h++)
                        {
                            uint outNbr = outEdges[C][nbr].vertexList[h];
                            Edge newEdge(i, outNbr, A);
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
                outEdges[g][i].OLD_END = outEdges[g][i].NEW_END;
                outEdges[g][i].NEW_END = outEdges[g][i].vertexList.size();
            }
        }
    }
}