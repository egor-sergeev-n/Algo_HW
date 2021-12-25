#pragma once
#include "IGraph.h"

class ArcGraph : public IGraph
{
public:
    ArcGraph(const int vertices_count);
    ArcGraph(const IGraph &graph);
    ~ArcGraph();

    void AddEdge(int from, int to);
    int VerticesCount() const;
    std::vector<int> GetNextVertices(int vertex) const;
    std::vector<int> GetPrevVertices(int vertex) const;

private:
    std::vector<std::pair<int, int>> edges;
    const int verticesCount;
};