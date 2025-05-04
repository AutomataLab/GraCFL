// #pragma once

// #include "utils/Graph.hpp"
// #include "utils/Edges.hpp"
// #include "utils/Types.hpp"
// #include "utils/Config.hpp"

// namespace gracfl 
// {
//     /**
//      * @class SolverBIGram
//      * @brief  Bidirectional CFL-reachability graph implementation and analysis using grammar-driven travesal and sliding pointers.
//      * 
//      * Inherits from Graph and adds support for bidirectional edge derivations.
//      * Maintains separate out-edges and in-edges adjacency lists, as well as a hashset to avoid duplicates.
//      */
//     class SolverBIGram : public SolverBase
//     {
//         Grammar* grammar_;
//         std::unique_ptr<Graph> graph_;
//     public:
//         SolverBIGram(Grammar* grammar, std::unique_ptr<Graph> graph);

//         /**
//          * @brief Executes the full bidirectional CFL-reachability analysis.
//          * @param grammar Grammar rules for generating new edges.
//          */
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