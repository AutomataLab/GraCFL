#include <iostream>
#include "solvers/SolverBIGram.hpp"

namespace gracfl 
{
    SolverBIGram::SolverBIGram(std::string graphfilepath, Grammar& grammar)
    : grammar_(grammar)
    , graph_(new Graph3DBi(graphfilepath, grammar))
    {
    }

    SolverBIGram::~SolverBIGram()
    {
        delete graph_;
    }

    void  SolverBIGram::runCFL()
    {
       
        uint itr = 0;
        bool terminate;
        auto& outEdges = graph_->outEdges_;
        auto& inEdges = graph_->inEdges_;
        auto& hashset = graph_->hashset_;
        auto& grammar2index = grammar_.grammar2index_;
        auto& grammar3indexLeft  = grammar_.grammar3indexLeft_;
        auto& grammar3indexRight = grammar_.grammar3indexRight_;
        auto labelSize = grammar_.getLabelSize();
        auto nodeSize = graph_->getNodeSize();

        addSelfEdges(); // add epsilon edges
        do {
            itr++;
            terminate = true;
            runSingleIteration(
                outEdges,
                inEdges, 
                hashset, 
                grammar2index,
                grammar3indexLeft,
                grammar3indexRight,
                labelSize,
                nodeSize,
                terminate);
            std::cout << "Iteration " << itr << std::endl;
        } while(!terminate);
    }

    void SolverBIGram::runSingleIteration(
        std::vector<std::vector<TemporalVector>>& outEdges,
        std::vector<std::vector<TemporalVector>>& inEdges,
        std::vector<std::vector<std::unordered_set<ull>>>& hashset,
        std::vector<std::vector<uint>>& grammar2index,
        std::vector<std::vector<std::pair<uint, uint>>>& grammar3indexLeft,
        std::vector<std::vector<std::pair<uint, uint>>>& grammar3indexRight,
        uint labelSize,
        uint nodeSize,
        bool& terminate)
    {
        // Derive new edges based on grammar rules
        for (uint g = 0; g < labelSize; g++)
        {
            for (uint i = 0; i < nodeSize; i++)
            {
                uint nbr;
                uint START_NEW = inEdges[g][i].OLD_END;
                uint END_NEW = inEdges[g][i].NEW_END;
                // Process new in-edges labeled g
                for (uint j = START_NEW; j < END_NEW; j++)
                {
                    uint inNbr = inEdges[g][i].vertexList[j];

                    // ------- Rule Type: A = B -------
                    for (uint m = 0; m < grammar2index[g].size(); m++)
                    {
                        uint A = grammar2index[g][m];
                        Edge newEdge(inNbr, i, A);
                        graph_->checkAndAddEdge(newEdge, terminate);
                    }

                    // ------- Rule Type: A = BC -------
                    for (uint m = 0; m < grammar3indexLeft[g].size(); m++)
                    {
                        uint C = grammar3indexLeft[g][m].first;
                        uint A = grammar3indexLeft[g][m].second;

                        uint START_OLD_OUT = 0;
                        uint END_NEW_OUT = outEdges[C][i].NEW_END;
                        for (uint h = START_OLD_OUT; h < END_NEW_OUT; h++)
                        {
                            uint nbr = outEdges[C][i].vertexList[h];
                            Edge newEdge(inNbr, nbr, A);
                            graph_->checkAndAddEdge(newEdge, terminate);
                        }
                    }
                }


                uint START_NEW_OUT = outEdges[g][i].OLD_END;
                uint END_NEW_OUT = outEdges[g][i].NEW_END;
                // Process new out-edges labeled g
                for (uint j = START_NEW_OUT; j < END_NEW_OUT; j++)
                {   
                    uint nbr = outEdges[g][i].vertexList[j];

                    // ------- Rule Type 3: A = CB -------
                    for (uint m = 0; m < grammar3indexRight[g].size(); m++)
                    {
                        uint C = grammar3indexRight[g][m].first;
                        uint A = grammar3indexRight[g][m].second;

                        uint START_OLD_IN = 0;
                        uint END_OLD_IN = inEdges[C][i].OLD_END;
                        for (uint h = START_OLD_IN; h < END_OLD_IN; h++)
                        {
                            uint inNbr = inEdges[C][i].vertexList[h];
                            Edge newEdge(inNbr, nbr, A);
                            graph_->checkAndAddEdge(newEdge, terminate);
                        }
                    }
                }
            }
        }

        // Update sliding pointers for next iteration
        for (uint g = 0; g < labelSize; g++)
        {
            for (int i = 0; i < nodeSize; i++)
            {
                outEdges[g][i].OLD_END = outEdges[g][i].NEW_END;
                outEdges[g][i].NEW_END = outEdges[g][i].vertexList.size();
                inEdges[g][i].OLD_END = inEdges[g][i].NEW_END; 
                inEdges[g][i].NEW_END = inEdges[g][i].vertexList.size();
            }
        }
    }

    void SolverBIGram::addSelfEdges()
    {
        for (int l = 0; l < grammar_.getRule1().size(); l++)
        {
            for (int i = 0; i < graph_->getNodeSize(); i++)
            {
                Edge edge(i, i, grammar_.grammar1_[l][0]);
                graph_->addSelfEdge(edge);
            }
        }
    }

    ull SolverBIGram::getEdgeCount()  
    { 
        return graph_->countEdge();
    }
}