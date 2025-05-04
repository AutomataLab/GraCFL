#include <vector>
#include <unordered_set>
#include "../Edges.hpp"
#include "Graph.hpp"
#include "../Types.hpp"

namespace gracfl {
    using Out3D = std::vector<std::vector<TemporalVector>>;
    using In3D = std::vector<std::vector<TemporalVector>>;
    using Hash3D = std::vector<std::vector<std::unordered_set<ull>>>;

    class Graph3DBi
    : public Graph<Out3D,In3D,Hash3D>
    {
    private:
        Out3D outEdges_;
        In3D inEdges_;
        Hash3D hashset_;

    public:
        Graph3DBi(std::string& graphfilepath, const Grammar& grammar);
        void initContainers() override;
        void addInitialEdges() override;
        void clearContainers() override;
        void addSelfEdge(Edge& edge) override;
        void addEdge(Edge& edge, bool& terminate) override;

        Out3D& getOutEdges() override { return outEdges_; }
        In3D& getInEdges() override { return inEdges_; }
        Hash3D& getHashset() override { return hashset_; }
    };
}
