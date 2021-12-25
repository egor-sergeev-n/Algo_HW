#include "ArcGraph.h"

ArcGraph::ArcGraph(const int vertices_count) : verticesCount(vertices_count)
{
}

ArcGraph::ArcGraph(const IGraph &graph) : verticesCount(graph.VerticesCount())
{
    for (int parent = 0; parent < graph.VerticesCount(); ++parent)
    {
        std::vector<int> children = graph.GetNextVertices(parent);
        for (int child : children)
            AddEdge(parent, child);
    }
}

ArcGraph::~ArcGraph()
{
}

void ArcGraph::AddEdge(int from, int to)
{
    edges.push_back(std::make_pair(from, to));
}

int ArcGraph::VerticesCount() const
{
    return verticesCount;
}

std::vector<int> ArcGraph::GetNextVertices(int vertex) const
{
    std::vector<int> result;
    for (int i = 0; i < edges.size(); ++i)
    {
        if (edges[i].first == vertex)
            result.push_back(edges[i].second);
    }
    return result;
}

std::vector<int> ArcGraph::GetPrevVertices(int vertex) const
{
    std::vector<int> result;
    for (int i = 0; i < edges.size(); ++i)
    {
        if (edges[i].second == vertex)
            result.push_back(edges[i].first);
    }
    return result;
}