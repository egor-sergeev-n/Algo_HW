#include <iostream>
#include <vector>
#include <queue>
#include "IGraph.h"
#include "ListGraph.h"

/* void testAnyGraph(const IGraph &graph)
{
}

void test()
{
    {
    }

    std::cout << "Tests passed\n";
} */

void dfs_aux(const IGraph &graph, const int vertex, std::vector<bool> &visited, void (*callback)(int v))
{
    visited[vertex] = true;
    callback(vertex);

    std::vector<int> children = graph.GetNextVertices(vertex);
    for (auto &child : children)
        if (!visited[child])
            dfs_aux(graph, child, visited, callback);
}

void dfs(const IGraph &graph, void (*callback)(int v))
{
    std::vector<bool> visited(graph.VerticesCount(), false);
    for (int vertex = 0; vertex < graph.VerticesCount(); ++vertex)
    {
        if (!visited[vertex])
        {
            dfs_aux(graph, vertex, visited, callback);
        }
    }
}

void bfs(const IGraph &graph, void (*callback)(int v))
{
    std::vector<bool> visited(graph.VerticesCount(), false);
    std::queue<int> q;
    for (int vertex = 0; vertex < graph.VerticesCount(); ++vertex)
    {
        if (!visited[vertex])
        {
            visited[vertex] = true;
            q.push(vertex);
            while (!q.empty())
            {
                int v = q.front();
                q.pop();
                callback(v);
                std::vector<int> children = graph.GetNextVertices(v);
                for (auto &child : children)
                    if (!visited[child])
                    {
                        visited[child] = true;
                        q.push(child);
                    }
            }
        }
    }
}

int main()
{
    IGraph *graph = new ListGraph(9);
    graph->AddEdge(0, 1);
    graph->AddEdge(1, 5);
    graph->AddEdge(6, 0);
    graph->AddEdge(1, 2);
    graph->AddEdge(2, 3);
    graph->AddEdge(3, 4);
    graph->AddEdge(4, 5);
    graph->AddEdge(0, 7);
    graph->AddEdge(0, 8);

    std::cout << "----------DFS----------\n";
    dfs(*graph, [](const int v)
        { std::cout << v << '\n'; });

    std::cout << "----------BFS----------\n";
    bfs(*graph, [](const int v)
        { std::cout << v << '\n'; });

    delete graph;
    return 0;
}