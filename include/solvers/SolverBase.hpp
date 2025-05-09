#pragma once

#include <iostream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <stdexcept>
#include <omp.h>
#include "utils/Edges.hpp"
#include "utils/Grammar.hpp"
#include "utils/graphs/Graph.hpp"
#include "utils/graphs/Graph3DBi.hpp"
#include "utils/graphs/Graph3DOut.hpp"
#include "utils/graphs/Graph3DIn.hpp"
#include "utils/Types.hpp"
#include "utils/Config.hpp"


namespace gracfl
{
    /**
     * @class SolverBase
     * @brief Base class for all CFL solver variants in the GraCFL framework.
     *
     * Provides a common interface for solving context-free language (CFL) reachability problems
     */
    class SolverBase 
    {
    public:
        /**
         * @brief Default constructor.
         */
        SolverBase() = default;

        /**
         * @brief Default constructor.
         */
        virtual ~SolverBase() = default;

        /**
         * @brief Pure virtual function to run the CFL solver.
         */
        virtual void runCFL() = 0;

        /**
         * @brief Pure virtual function to retrieve the total number of edges discovered/processed.
         * @return Total number of CFL-reachable edges in the graph.
         */
        virtual ull getEdgeCount() = 0;
    
         /**
         * @brief Pure virtual function to retrieve the final graph as a 2D vector of unordered_sets.
         * @return The graph in vertex and label indexed-wise outgoing adjacency list format.
         */
        virtual std::vector<std::vector<std::unordered_set<ull>>> getGraph() = 0;

        /**
         * @brief Converts an incoming edge hashset (inHashset) to an outgoing edge hashset (outHashset).
         *
         * Useful when a backward traversal has been executed and a forward representation is needed.
         *
         * @param inHashset A 2D vector representing incoming edges: node × label → {source nodes}.
         * @return A 2D vector representing outgoing edges: node × label → {destination nodes}.
         */
        std::vector<std::vector<std::unordered_set<ull>>> convertInHashsetToOutHashset(
            std::vector<std::vector<std::unordered_set<ull>>>& inHashset
        )
        {
            std::vector<std::vector<std::unordered_set<ull>>> outHashset(inHashset.size());
            for (uint i = 0; i < inHashset.size(); i++)
            {
                outHashset[i].resize(inHashset[i].size());
            }

            for (uint i = 0; i < inHashset.size(); i++)
            {
                for (uint j = 0; j < inHashset[i].size(); j++)
                {
                    for (auto& elem : inHashset[i][j])
                    {
                        outHashset[i][elem].insert(j);
                    }
                }
            }

            return outHashset;
        }
    };
}