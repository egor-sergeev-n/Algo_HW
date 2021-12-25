#include "SetGraph.h"

SetGraph::SetGraph(const int vertices_count) : vertices(vertices_count)
{
}

SetGraph::SetGraph(const IGraph &graph) : vertices(graph.VerticesCount())
{
    for (int parent = 0; parent < graph.VerticesCount(); ++parent)
    {
        std::vector<int> children = graph.GetNextVertices(parent);
        for (int child : children)
            AddEdge(parent, child);
    }
}

SetGraph::~SetGraph()
{
}

void SetGraph::AddEdge(int from, int to)
{
    vertices[from].insert(to);
}

int SetGraph::VerticesCount() const
{
    return vertices.size();
}

std::vector<int> SetGraph::GetNextVertices(int vertex) const
{
    std::vector<int> result;
    for (int i = 0; i < vertices.size(); ++i)
    {
        if (vertices[vertex].find(i) != vertices[vertex].end())
            result.push_back(i);
    }
    return result;
}

std::vector<int> SetGraph::GetPrevVertices(int vertex) const
{
    std::vector<int> result;
    for (int i = 0; i < vertices.size(); ++i)
    {
        if (vertices[i].find(vertex) != vertices[i].end())
            result.push_back(i);
    }
    return result;
}