#include <iostream>
#include <omp.h>
#include "solvers/SolverBWTopoParallel.hpp"

namespace gracfl 
{
    SolverBWTopoParallel::SolverBWTopoParallel(std::string graphfilepath, Grammar& grammar, uint numOfThreads)
    : SolverBWTopo(graphfilepath, grammar)
    {
        numOfThreads_ = numOfThreads;
    }

    void SolverBWTopoParallel::runCFL()
    {
        uint itr = 0;
        bool terminate;
        auto& inEdges = graph_->inEdges_;
        auto& inHashset = graph_->inHashset_;
        auto& grammar2index = grammar_.grammar2index_;
        auto& grammar3index  = grammar_.grammar3index_;
        auto labelSize = grammar_.getLabelSize();
        auto nodeSize = graph_->getNodeSize();

        addSelfEdges(); // add epsilon edges
        do {
            itr++;
            terminate = true;
            runSingleIterationParallel(
                inEdges, 
                inHashset, 
                grammar2index,
                grammar3index,
                labelSize,
                nodeSize,
                terminate);
            std::cout << "Iteration " << itr << std::endl;
        } while(!terminate);
    }


    void SolverBWTopoParallel::runSingleIterationParallel(
        std::vector<TemporalVectorWithLbldVtx>& inEdges,
        std::vector<std::vector<std::unordered_set<ull>>>& inHashset,
        std::vector<std::vector<uint>>& grammar2index,
        std::vector<std::vector<uint>>& grammar3index,
        uint labelSize,
        uint nodeSize,
        bool& terminate)
    {
        // Derive new edges based on grammar rules
        #pragma omp parallel for schedule(static) num_threads(numOfThreads_)
		for (uint i = 0; i < nodeSize; i++)
		{
			LbldVtx nbr;
			uint START_NEW_OUT = inEdges[i].OLD_END;
			uint END_NEW_OUT = inEdges[i].NEW_END;

			for (uint j = START_NEW_OUT; j < END_NEW_OUT; j++)
			{
				nbr = inEdges[i].vertexList[j];
				std::vector<uint> leftLabels = grammar2index[nbr.label];

                // ------- Rule Type: A = B -------
				for (uint g = 0; g < leftLabels.size(); g++)
				{
                    Edge newEdge(nbr.vtx, i, leftLabels[g]);
                    // check if the edge already exists and add to the graph
                    graph_->checkAndAddEdge(newEdge, terminate);
				}

				uint START_OLD = 0;
				uint END_NEW = inEdges[nbr.vtx].NEW_END;

				for (uint h = START_OLD; h < END_NEW; h++)
				{
					LbldVtx outInNbr = inEdges[nbr.vtx].vertexList[h];
					std::vector<uint> leftLabels = grammar3index[outInNbr.label * labelSize +  nbr.label];

					for (uint g = 0; g < leftLabels.size(); g++)
					{
                        Edge newEdge(outInNbr.vtx, i, leftLabels[g]);
                        // check if the edge already exists and add to the graph
                        graph_->checkAndAddEdge(newEdge, terminate);
					}
				}
			}

			uint OLD_START_IN = 0;
			uint OLD_END_IN = inEdges[i].OLD_END;
			for (uint j = OLD_START_IN; j < OLD_END_IN; j++)
			{
				LbldVtx nbr = inEdges[i].vertexList[j];

				uint NEW_START_IN = inEdges[nbr.vtx].OLD_END;
				uint NEW_END_IN = inEdges[nbr.vtx].NEW_END;
				for (uint h = NEW_START_IN; h < NEW_END_IN; h++)
				{
					LbldVtx outInNbr = inEdges[nbr.vtx].vertexList[h];
					std::vector<uint> leftLabels = grammar3index[outInNbr.label * labelSize + nbr.label];

					for (uint g = 0; g < leftLabels.size(); g++)
					{
                        Edge newEdge(outInNbr.vtx, i, leftLabels[g]);
                        // check if the edge already exists and add to the graph
                        graph_->checkAndAddEdge(newEdge, terminate);
					}
				}
			}
		}

        // ----------------- Update Sliding Pointers -----------------
        #pragma omp parallel for schedule(static) num_threads(numOfThreads_)
		for (int i = 0; i < nodeSize; i++)
		{
			inEdges[i].OLD_END = inEdges[i].NEW_END;
			inEdges[i].NEW_END = inEdges[i].vertexList.size();
		}
    }
}