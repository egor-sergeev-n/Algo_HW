//2. Количество различных путей

#include <iostream>
#include <vector>
#include <queue>
#include <sstream>
#include "assert.h"

class Graph
{
public:
    Graph(const size_t vert_count) : vertices(vert_count)
    {
    }

    ~Graph() {}

    size_t verticesCount() const
    {
        return vertices.size();
    }

    void addEdge(const uint16_t vertex_1, const uint16_t vertex_2)
    {
        vertices[vertex_1].push_back(vertex_2);
        vertices[vertex_2].push_back(vertex_1);
    }

    const std::vector<uint16_t> &getNextVertices(const uint16_t vertex) const
    {
        return vertices[vertex];
    }

private:
    std::vector<std::vector<uint16_t>> vertices;
};

size_t shortestPathCount(const Graph &graph, const uint16_t from, const uint16_t to)
{
    std::vector<bool> visited(graph.verticesCount(), false);
    std::vector<size_t> path_count(graph.verticesCount(), 0);
    std::vector<size_t> dist(graph.verticesCount(), 0);
    path_count[from] = 1;
    std::queue<uint16_t> q;
    q.push(from);
    while (!q.empty())
    {
        uint16_t cur = q.front();
        q.pop();
        if (cur == to)
            break;
        const std::vector<uint16_t> children = graph.getNextVertices(cur);
        for (const uint16_t &child : children)
        {
            if (dist[child] == 0 || dist[child] > dist[cur] + 1)
            {
                dist[child] = dist[cur] + 1;
                path_count[child] = path_count[cur];
            }
            else if (dist[child] == dist[cur] + 1)
                path_count[child] += path_count[cur];

            if (!visited[child])
            {
                visited[child] = true;
                q.push(child);
            }
        }
    }
    return path_count[to];
}

void processRequest(std::istream &in, std::ostream &out)
{
    size_t v, n;
    in >> v >> n;
    Graph graph(v);
    for (size_t i = 0; i < n; ++i)
    {
        uint16_t v1, v2;
        in >> v1 >> v2;
        graph.addEdge(v1, v2);
    }

    uint16_t from, to;
    in >> from >> to;
    out << shortestPathCount(graph, from, to) << '\n';
}

void testGraph()
{
    Graph graph(5);

    graph.addEdge(0, 2);
    graph.addEdge(1, 0);
    graph.addEdge(1, 2);
    graph.addEdge(1, 3);
    graph.addEdge(2, 3);
    graph.addEdge(3, 4);
    graph.addEdge(4, 2);

    assert(graph.getNextVertices(0) == std::vector<uint16_t>({2, 1}));
    assert(graph.getNextVertices(1) == std::vector<uint16_t>({0, 2, 3}));
    assert(graph.getNextVertices(2) == std::vector<uint16_t>({0, 1, 3, 4}));
    assert(graph.getNextVertices(3) == std::vector<uint16_t>({1, 2, 4}));
    assert(graph.getNextVertices(4) == std::vector<uint16_t>({3, 2}));
}

void testShortestPathCount()
{
    {
        Graph graph(7);
        graph.addEdge(0, 1);
        graph.addEdge(0, 2);

        graph.addEdge(1, 3);
        graph.addEdge(2, 3);

        graph.addEdge(3, 4);
        graph.addEdge(3, 5);

        graph.addEdge(4, 6);
        graph.addEdge(5, 6);

        assert(shortestPathCount(graph, 0, 6) == 4);
    }

    {
        Graph graph(10);
        graph.addEdge(0, 1);
        graph.addEdge(0, 2);

        graph.addEdge(1, 3);
        graph.addEdge(1, 4);

        graph.addEdge(2, 5);
        graph.addEdge(2, 6);

        graph.addEdge(3, 7);
        graph.addEdge(4, 7);

        graph.addEdge(5, 8);
        graph.addEdge(6, 8);

        graph.addEdge(7, 9);
        graph.addEdge(8, 9);

        assert(shortestPathCount(graph, 0, 9) == 4);
    }
}

void testRequest()
{
    {
        std::stringstream in;
        in << "4\n5\n0 1\n0 2\n1 2\n1 3\n2 3\n0 3";
        std::stringstream out;
        processRequest(in, out);
        assert(out.str() == "2\n");
    }

    {
        std::stringstream in;
        in << "10\n12\n0 1\n0 2\n1 3\n1 4\n2 5\n2 6\n3 7\n4 7\n5 8\n6 8\n7 9\n8 9\n0 9";
        std::stringstream out;
        processRequest(in, out);
        assert(out.str() == "4\n");
    }
}

void testAll()
{
    testGraph();
    testShortestPathCount();
    testRequest();
    std::cout << "ALL TESTS PASSED\n";
}

int main()
{
    testAll();
    //processRequest(std::cin, std::cout);
    return 0;
}