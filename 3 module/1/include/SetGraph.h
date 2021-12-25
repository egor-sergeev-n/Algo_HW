#pragma once
#include "IGraph.h"
#include <unordered_set>

class SetGraph : public IGraph
{
public:
    SetGraph(const int vertices_count);
    SetGraph(const IGraph &graph);
    ~SetGraph();

    void AddEdge(int from, int to);
    int VerticesCount() const;
    std::vector<int> GetNextVertices(int vertex) const;
    std::vector<int> GetPrevVertices(int vertex) const;

private:
    std::vector<std::unordered_set<int>> vertices;
};