// #pragma once

// #include <string> 
// #include <vector> 
// #include <unordered_set> 
// #include <cstdint>
// #include "Grammar.hpp"
// #include "Edges.hpp" 
// #include "Types.hpp"                

// namespace gracfl 
// {
//     /**
//      * @class Graph
//      * @brief Graph container supporting CFL reachability preprocessing.
//      *
//      * This class represents the input graph as a list of edges, tracks
//      * the number of nodes and edges, and provides methods to load the graph
//      * from file, clear edges, and count derived edges via a helper.
//      */
//     class Graph
//     {
//     private:
//         /// Total number of nodes in the graph
//         uint numNodes_ = 0;
//         /// Total number of edges in the graph
//         uint numEdges_ = 0;
//         /// Flat list of edges read from input
//         std::vector<Edge> edges_;
//         /// Path to the input graph file
//         std::string graphfilepath_;
//     public:
//         /**
//          * @brief Construct a Graph from a file and a grammar.
//          * @param graphfilepath Path to the graph file to load.
//          * @param grammar       Grammar used for CFL reachability setup.
//          */
//         explicit Graph(std::string& graphfilepath, const Grammar& grammar);

//         /**
//          * @brief Load or reload the graph data from file.
//          * 
//          * Parses the file at @p graphfilepath according to the provided
//          * @p grammar, populating the internal edge list and updating
//          * node/edge counts.
//          *
//          * @param graphfilepath Path to the graph file.
//          * @param grammar       Grammar guiding edge interpretation.
//          */
//         void loadGraphFile(std::string& graphfilepath, const Grammar& grammar);

//         /**
//          * @brief Helper to count edges based on a hashset structure.
//          *
//          * Given a nested vector of unordered_sets representing
//          * derived edges per source and label, this returns the total
//          * count of unique edges.
//          *
//          * @param hashset Nested vector [source][label] → set of targets.
//          * @return Total number of unique edges across all sets.
//          */
//         ull countEdgeHelper(std::vector<std::vector<std::unordered_set<ull>>>& hashset);

//         /**
//          * @brief Get the raw list of edges.
//          * @return Reference to the vector of Edge.
//          */
//         inline std::vector<Edge>&  getEdges() { return edges_; }

//         /**
//          * @brief Remove all edges from the graph.
//          */
//         inline void clearEdges() { edges_.clear(); }

//         /**
//          * @brief Get the number of nodes.
//          * @return Number of nodes in the graph.
//          */
//         inline size_t  getNodeSize() { return numNodes_; }

//         /**
//          * @brief Get the number of edges.
//          * @return Number of edges in the graph.
//          */
//         inline size_t  getEdgeSize() { return numEdges_; }

//         /**
//          * @brief Get the path of the graph file.
//          * @return Filepath used to load the graph.
//          */
//         inline std::string getGraphfilepath(){ return graphfilepath_; } 

//         /**
//          * @brief Perform CFL-reachability solving on this graph.
//          * 
//          * Concrete subclasses must implement how the graph is
//          * processed according to the grammar to derive new edges.
//          *
//          * @param grammar Grammar guiding the CFL reachability.
//          */
//         virtual void solve(const Grammar& grammar) = 0;

//         /**
//          * @brief Count total edges after CFL reachability.
//          * 
//          * Concrete subclasses must implement
//          * @return Total number of edges after processing.
//          */
//         virtual ull countEdge() = 0;


//          /**
//          * @brief Initializes adjacency lists + hashsets with edges from the input graph.
//          */
//         virtual void addInitialEdges() = 0;


//         /**
//          * @brief Accessor for outgoing-edge adjacency list.
//          * @returns Reference to outEdges_.
//          */
//         virtual std::vector<std::vector<TemporalVector>>& getOutEdges() {
//             std::vector<std::vector<TemporalVector>> outEdges_;
//             return outEdges_; }



//     };
// }

