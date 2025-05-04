#pragma once

#include "../Edges.hpp"
#include "../Grammar.hpp"

// Graph.hpp
namespace gracfl {
    template<typename OutEdges, typename InEdges, typename HashSets>
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
        ull countEdge(std::vector<std::vector<std::unordered_set<ull>>>& hashset);

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

        inline size_t getLabelSize() { return numLabels_; }

        // hooks:
        virtual void initContainers()    = 0;
        virtual void addInitialEdges()   = 0;
        virtual void clearContainers()   = 0;
        virtual void addSelfEdge(Edge& edge) = 0;
        virtual void addEdge(Edge& edge, bool& terminate) = 0;

        // now these all have the *same* signature:
        virtual OutC& getOutEdges() = 0;
        virtual InC& getInEdges()  = 0;
        virtual HashC& getHashset()  = 0;
    };
}
