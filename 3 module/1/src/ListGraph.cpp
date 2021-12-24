#include "ListGraph.h"
#include <algorithm>

ListGraph::ListGraph(const int vertices_count) : vertices(vertices_count)
{
}

ListGraph::ListGraph(const IGraph &graph) : vertices(graph.VerticesCount())
{
    for (int parent = 0; parent < graph.VerticesCount(); ++parent)
    {
        std::vector<int> children = graph.GetNextVertices(parent);
        for (int child : children)
            AddEdge(parent, child);
    }
}

ListGraph::~ListGraph()
{
}

void ListGraph::AddEdge(int from, int to)
{
    vertices[from].push_back(to);
}

int ListGraph::VerticesCount() const
{
    return vertices.size();
}

std::vector<int> ListGraph::GetNextVertices(int vertex) const
{
    return vertices[vertex];
}

std::vector<int> ListGraph::GetPrevVertices(int vertex) const
{
    std::vector<int> result;
    for (int parent = 0; parent < vertices.size(); ++parent)
    {
        if (std::find(vertices[parent].begin(),
                      vertices[parent].end(),
                      vertex) != vertices[parent].end())
            result.push_back(parent);
    }
    return result;
}