#include <iostream>
#include <vector>
#include "solvers/SolverFWTopo.hpp"

namespace gracfl 
{
    SolverFWTopo::SolverFWTopo(std::string graphfilepath, Grammar& grammar)
        : grammar_(grammar)
        , graph_(new Graph2DOut(graphfilepath, grammar))
    {
    }

    SolverFWTopo::~SolverFWTopo()
    {
        delete graph_;
    }

    void SolverFWTopo::runCFL()
    {
        uint itr = 0;
        bool terminate;
        auto& outEdges = graph_->outEdges_;
        auto& hashset = graph_->hashset_;
        auto& grammar2index = grammar_.grammar2index_;
        auto& grammar3index = grammar_.grammar3index_;
        auto labelSize = grammar_.getLabelSize();
        auto nodeSize = graph_->getNodeSize();

        addSelfEdges(); // Add epsilon edges
        do {
            itr++;
            terminate = true;
            runSingleIteration(
                outEdges,
                hashset, 
                grammar2index,
                grammar3index,
                labelSize,
                nodeSize,
                terminate
            );
            std::cout << "Iteration " << itr << std::endl;
        } while (!terminate);
    }

    void SolverFWTopo::runSingleIteration(
        std::vector<TemporalVectorWithLbldVtx>& outEdges,
        std::vector<std::vector<std::unordered_set<ull>>>& hashset,
        std::vector<std::vector<uint>>& grammar2index,
        std::vector<std::vector<uint>>& grammar3index,
        uint labelSize,
        uint nodeSize,
        bool& terminate)
    {
        // For each grammar rule like A --> B
        for (uint i = 0; i < nodeSize; i++) {
            LbldVtx nbr;
            // The valid index range is [START_NEW, END_NEW-1]
            uint START_NEW_OUT = outEdges[i].OLD_END;
            uint END_NEW_OUT = outEdges[i].NEW_END;

            // For each new edge
            for (uint j = START_NEW_OUT; j < END_NEW_OUT; j++) {
                nbr = outEdges[i].vertexList[j];
                // If the edge to the neighbor is labeled with B
                std::vector<uint> leftLabels = grammar2index[nbr.label];

                for (uint g = 0; g < leftLabels.size(); g++) {
                    Edge newEdge(i, nbr.vtx, leftLabels[g]);
                    // Check if the edge already exists and add to graph
                    graph_->checkAndAddEdge(newEdge, terminate);
                }

                uint START_OLD = 0;
                uint END_NEW = outEdges[nbr.vtx].NEW_END;
                for (uint h = START_OLD; h < END_NEW; h++) {
                    LbldVtx outNbr = outEdges[nbr.vtx].vertexList[h];
                    std::vector<uint> leftLabels = grammar3index[nbr.label * labelSize + outNbr.label];

                    for (uint g = 0; g < leftLabels.size(); g++) {
                        Edge newEdge(i, outNbr.vtx, leftLabels[g]);
                        // Check if the edge already exists and add to graph
                        graph_->checkAndAddEdge(newEdge, terminate);
                    }
                }
            }

            uint OLD_START_OUT = 0;
            uint OLD_END_OUT = outEdges[i].OLD_END;
            for (uint j = OLD_START_OUT; j < OLD_END_OUT; j++) {
                LbldVtx nbr = outEdges[i].vertexList[j];

                uint NEW_START_OUT = outEdges[nbr.vtx].OLD_END;
                uint NEW_END_OUT = outEdges[nbr.vtx].NEW_END;
                for (uint h = NEW_START_OUT; h < NEW_END_OUT; h++) {
                    LbldVtx outNbr = outEdges[nbr.vtx].vertexList[h];
                    std::vector<uint> leftLabels = grammar3index[nbr.label * labelSize + outNbr.label];

                    for (uint g = 0; g < leftLabels.size(); g++) {
                        Edge newEdge(i, outNbr.vtx, leftLabels[g]);
                        // Check if the edge already exists and add to graph
                        graph_->checkAndAddEdge(newEdge, terminate);
                    }
                }
            }
        }

        // Update the pointers
        for (int i = 0; i < nodeSize; i++) {
            outEdges[i].OLD_END = outEdges[i].NEW_END;
            outEdges[i].NEW_END = outEdges[i].vertexList.size();
        }
    }

    void SolverFWTopo::addSelfEdges()
    {
        for (int l = 0; l < grammar_.getRule1().size(); l++) {
            for (int i = 0; i < graph_->getNodeSize(); i++) {
                Edge edge(i, i, grammar_.grammar1_[l][0]);
                graph_->addSelfEdge(edge);
            }
        }
    }

    ull SolverFWTopo::getEdgeCount()  
    { 
        return graph_->countEdge();
    }
}