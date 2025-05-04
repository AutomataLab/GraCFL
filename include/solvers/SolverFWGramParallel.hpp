// #pragma once

// #include "utils/Graph.hpp"
// #include "utils/Edges.hpp"
// #include "utils/Types.hpp"
// #include "utils/Config.hpp"
// #include "SolverFWGram.hpp"

// namespace gracfl 
// {
//     /**
//      * @class SolverFWGramParallel
//      * @brief  Parallel Forward directional CFL-reachability graph implementation and analysis using grammar-driven travesal and sliding pointers.
//      * 
//      * Inherits from SolverFWGram and adds support for parallel forward directional edge derivations.
//      */
//     class SolverFWGramParallel : public SolverFWGram 
//     {
//         uint numOfThreads_ = 1;
//     public:
//         SolverFWGramParallel(Grammar* grammar, std::unique_ptr<Graph> graph, uint numOfThreads);

//         /**
//          * @brief Executes the full parallel forward-directional CFL-reachability analysis.
//          * @param grammar Grammar rules for generating new edges.
//          */
//         void solve(const Grammar& grammar) override;

//          /**
//          * @brief Performs one iteration of edge derivation.
//          * 
//          * In this iteration, derivations are parallelized across vertices
//          * to leverage multi-threaded performance.
//          * @param grammar   Grammar rules for edge derivations.
//          * @param terminate Flag set to false if new edges were added.
//          */
//         void runSingleIterationParallelLabel(bool& terminate, uint numOfThreads);

//         /**
//          * @brief Performs one iteration of edge derivation.
//          * 
//          * In this iteration, derivations are parallelized across vertices
//          * to leverage multi-threaded performance.
//          * @param grammar   Grammar rules for edge derivations.
//          * @param terminate Flag set to false if new edges were added.
//          */
//         void runSingleIterationParallel(bool& terminate, uint numOfThreads);
//     };
// }