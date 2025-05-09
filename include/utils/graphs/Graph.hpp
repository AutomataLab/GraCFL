#pragma once

#include "../Edges.hpp"
#include "../Grammar.hpp"

// Graph.hpp
namespace gracfl {
    class Graph {
    private:
         /// Total number of nodes in the graph
         uint numNodes_ = 0;
         /// Total number of edges in the graph
         uint numEdges_ = 0;
         /// Total number of unique edge labels in the graph
         uint numLabels_ = 0;
         /// Flat list of edges read from input
         std::vector<Edge> edges_;
    public:
        Graph() = default;
        Graph(std::string& graphfilepath, const Grammar& grammar);
        virtual ~Graph() = default;
        void loadGraphFile(std::string& graphfilepath, const Grammar& grammar);
        ull countEdgeHelper(std::vector<std::vector<std::unordered_set<ull>>>& hashset);

        /**
         * @brief Get the number of nodes.
         * @return Number of nodes in the graph.
         */
        inline size_t getNodeSize() { return numNodes_; }

        /**
         * @brief Get the number of edges.
         * @return Number of edges in the graph.
         */
        inline size_t getEdgeSize() { return numEdges_; }

        /**
         * @brief Get the number of labels.
         * @return Number of unique labels in the graph.
         */
        inline size_t getLabelSize() { return numLabels_; }

        /**
         * @brief Get the edges of the graph.
         * @return Vector of edges in the graph.
         */
        inline std::vector<Edge>& getEdges() { return edges_; }
    };
}
