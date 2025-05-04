// #pragma once

// #include "utils/Graph.hpp"
// #include "utils/Edges.hpp"
// #include "utils/Types.hpp"
// #include "utils/Config.hpp"

// namespace gracfl 
// {
//     /**
//      * @class SolverBWGram
//      * @brief  Backward directional CFL-reachability graph implementation and analysis using grammar-driven travesal and sliding pointers.
//      * 
//      * Inherits from Graph and adds support for backward directional edge derivations.
//      * Maintains  in-edges adjacency list, as well as a hashset to avoid duplicates.
//      */
//     class SolverBWGram : public SolverBase 
//     {
//         Grammar* grammar_;
//         std::unique_ptr<Graph> graph_;
//     public:
       
//         SolverBWGram(Grammar* grammar_, std::unique_ptr<Graph> graph_);

//         void runCFL() override;

//         /**
//          * @brief Performs one iteration of edge derivation.
//          * @param grammar   Grammar rules for edge derivations.
//          * @param terminate Flag set to false if new edges were added.
//          */
//         void runSingleIteration(bool& terminate);

//         /**
//          * @brief Adds self-loop edges (epsilon rules) for all nodes.
//          * @param grammar Grammar rules for edge derivations.
//          */
//         void addSelfEdges();
//     };
// }