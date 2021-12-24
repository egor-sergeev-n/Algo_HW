#pragma once
#include "IGraph.h"

class ListGraph : public IGraph
{
public:
    ListGraph(const int vertices_count);
    ListGraph(const IGraph &graph);
    ~ListGraph();

    void AddEdge(int from, int to);
    int VerticesCount() const;
    std::vector<int> GetNextVertices(int vertex) const;
    std::vector<int> GetPrevVertices(int vertex) const;

private:
    std::vector<std::vector<int>> vertices;
};