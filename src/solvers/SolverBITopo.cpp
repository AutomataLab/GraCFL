#include <iostream>
#include "solvers/SolverBITopo.hpp"

namespace gracfl 
{
    SolverBITopo::SolverBITopo(std::string graphfilepath, Grammar& grammar)
    : grammar_(grammar)
    , graph_(new Graph2DBi(graphfilepath, grammar))
    {
    }

    SolverBITopo::~SolverBITopo()
    {
        delete graph_;
    }

    void  SolverBITopo::runCFL()
    {
       
        uint itr = 0;
        bool terminate;
        auto& outEdges = graph_->outEdges_;
        auto& inEdges = graph_->inEdges_;
        auto& hashset = graph_->hashset_;
        auto& grammar2index = grammar_.grammar2index_;
        auto& grammar3index  = grammar_.grammar3index_;
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
                grammar3index,
                labelSize,
                nodeSize,
                terminate);
            std::cout << "Iteration " << itr << std::endl;
        } while(!terminate);
    }

    void SolverBITopo::runSingleIteration(
        std::vector<TemporalVectorWithLbldVtx>& outEdges,
        std::vector<TemporalVectorWithLbldVtx>& inEdges,
        std::vector<std::vector<std::unordered_set<ull>>>& hashset,
        std::vector<std::vector<uint>>& grammar2index,
        std::vector<std::vector<uint>>& grammar3index,
        uint labelSize,
        uint nodeSize,
        bool& terminate)
    {
        // Derive new edges based on grammar rules
        for (uint i = 0; i < nodeSize; i++)
		{
			LbldVtx inNbr;
			uint START_NEW_IN = inEdges[i].OLD_END;
			uint END_NEW_IN = inEdges[i].NEW_END;

			// for each new edge
			for (uint j = START_NEW_IN; j < END_NEW_IN; j++)
			{
				inNbr = inEdges[i].vertexList[j];

				// ------- Rule Type: A = B -------
				std::vector<uint> leftLabels = grammar2index[inNbr.label];
				for (uint g = 0; g < leftLabels.size(); g++)
				{
                    Edge newEdge(inNbr.vtx, i, leftLabels[g]);
					graph_->checkAndAddEdge(newEdge, terminate);
				}

				// ------- Rule Type: A = BC -------
				uint START_OLD = 0;
				uint END_NEW = outEdges[i].NEW_END;
				for (uint h = START_OLD; h < END_NEW; h++)
				{
					LbldVtx outNbr = outEdges[i].vertexList[h];
					std::vector<uint> leftLabels = grammar3index[inNbr.label * labelSize + outNbr.label];
					for (uint g = 0; g < leftLabels.size(); g++)
					{
                        Edge newEdge(inNbr.vtx, outNbr.vtx, leftLabels[g]);
                        graph_->checkAndAddEdge(newEdge, terminate);
					}
				}
			}

			uint NEW_START_OUT = outEdges[i].OLD_END;
			uint NEW_END_OUT = outEdges[i].NEW_END;
			for (uint j = NEW_START_OUT; j < NEW_END_OUT; j++)
			{
				LbldVtx outNbr = outEdges[i].vertexList[j];

				// ------- Rule Type: A = CB -------
				uint OLD_START_IN = 0;
				uint OLD_END_IN = inEdges[i].OLD_END;
				for (uint h = OLD_START_IN; h < OLD_END_IN; h++)
				{
					LbldVtx inNbr = inEdges[i].vertexList[h];
					std::vector<uint> leftLabels = grammar3index[inNbr.label * labelSize + outNbr.label];

					for (uint g = 0; g < leftLabels.size(); g++)
					{
                        Edge newEdge(inNbr.vtx, outNbr.vtx, leftLabels[g]);
                        graph_->checkAndAddEdge(newEdge, terminate);
					}
				}
			}
		}

		 // ----------------- Update Sliding Pointers -----------------
		for (int i = 0; i < nodeSize; i++)
		{
			outEdges[i].OLD_END = outEdges[i].NEW_END;
			inEdges[i].OLD_END = inEdges[i].NEW_END;
			outEdges[i].NEW_END = outEdges[i].vertexList.size();
			inEdges[i].NEW_END = inEdges[i].vertexList.size();
		}
    }

    void SolverBITopo::addSelfEdges()
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

    ull SolverBITopo::getEdgeCount()  
    { 
        return graph_->countEdge();
    }
}