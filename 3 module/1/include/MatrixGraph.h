#pragma once
#include "IGraph.h"

class MatrixGraph : public IGraph
{
public:
    MatrixGraph(const int vertices_count);
    MatrixGraph(const IGraph &graph);
    ~MatrixGraph();

    void AddEdge(int from, int to);
    int VerticesCount() const;
    std::vector<int> GetNextVertices(int vertex) const;
    std::vector<int> GetPrevVertices(int vertex) const;

private:
    std::vector<std::vector<int>> matrix;
};