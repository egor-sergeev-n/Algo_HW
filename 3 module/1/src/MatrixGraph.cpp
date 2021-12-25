#include "MatrixGraph.h"

MatrixGraph::MatrixGraph(const int vertices_count) : matrix(vertices_count, std::vector<int>(vertices_count, 0))
{
}

MatrixGraph::MatrixGraph(const IGraph &graph) : matrix(graph.VerticesCount(), std::vector<int>(graph.VerticesCount(), 0))
{
    for (int parent = 0; parent < graph.VerticesCount(); ++parent)
    {
        std::vector<int> children = graph.GetNextVertices(parent);
        for (int child : children)
            AddEdge(parent, child);
    }
}

MatrixGraph::~MatrixGraph()
{
}

void MatrixGraph::AddEdge(int from, int to)
{
    matrix[from][to] = 1;
}

int MatrixGraph::VerticesCount() const
{
    return matrix.size();
}

std::vector<int> MatrixGraph::GetNextVertices(int vertex) const
{
    std::vector<int> result;
    for (int i = 0; i < matrix.size(); ++i)
    {
        if (matrix[vertex][i] == 1)
            result.push_back(i);
    }
    return result;
}

std::vector<int> MatrixGraph::GetPrevVertices(int vertex) const
{
    std::vector<int> result;
    for (int i = 0; i < matrix.size(); ++i)
    {
        if (matrix[i][vertex] == 1)
            result.push_back(i);
    }
    return result;
}