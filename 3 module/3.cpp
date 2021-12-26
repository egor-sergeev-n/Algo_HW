#include <iostream>
#include <sstream>
#include <vector>
#include <set>
#include "assert.h"

typedef std::vector<std::pair<int, int>> ChildrenList;

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

    void addEdge(const int from, const int to, const int weight)
    {
        vertices[from].push_back(std::make_pair(to, weight));
    }

    const ChildrenList &getNextVertices(const int vertex) const
    {
        return vertices[vertex];
    }

    std::vector<int> dijkstra(const int from)
    {
        std::vector<int> dist(verticesCount(), -1);
        std::set<std::pair<int, int>> s;
        s.insert(std::make_pair(0, from));
        while (!s.empty())
        {
            auto it = s.begin();
            std::pair<int, int> pair = *it;
            s.erase(it);
            int cur_vertex = pair.second;
            int cur_dist = pair.first;

            for (const std::pair<int, int> &edge : getNextVertices(cur_vertex))
            {
                int next_vertex = edge.first;
                int weight = edge.second;
                if (dist[next_vertex] == -1)
                {
                    dist[next_vertex] = cur_dist + weight;
                    s.insert(std::make_pair(dist[next_vertex], next_vertex));
                }
                else if (dist[next_vertex] > cur_dist + weight)
                {
                    s.erase(std::make_pair(dist[next_vertex], next_vertex));
                    dist[next_vertex] = cur_dist + weight;
                    s.insert(std::make_pair(dist[next_vertex], next_vertex));
                }
            }
        }

        return dist;
    }

private:
    std::vector<ChildrenList> vertices;
};

void processRequest(std::istream &in, std::ostream &out)
{
    size_t N, M;
    in >> N >> M;
    Graph graph(N);
    for (size_t i = 0; i < M; ++i)
    {
        int v1, v2, time;
        in >> v1 >> v2 >> time;
        graph.addEdge(v1, v2, time);
        graph.addEdge(v2, v1, time);
    }

    int from, to;
    in >> from >> to;
    out << graph.dijkstra(from)[to] << '\n';
}

void testGraph()
{
    Graph graph(4);

    graph.addEdge(0, 1, 9);
    graph.addEdge(0, 2, 2);
    graph.addEdge(0, 3, 6);
    graph.addEdge(2, 3, 3);
    graph.addEdge(3, 1, 2);

    assert(graph.verticesCount() == 4);
    assert(graph.getNextVertices(0) == ChildrenList({{1, 9}, {2, 2}, {3, 6}}));
    assert(graph.getNextVertices(2) == ChildrenList({{3, 3}}));
    assert(graph.getNextVertices(3) == ChildrenList({{1, 2}}));
}

void testDijkstra()
{
    // Графы 2, слайд 19
    Graph graph(9);

    graph.addEdge(0, 1, 9);
    graph.addEdge(0, 2, 2);
    graph.addEdge(0, 3, 6);

    graph.addEdge(1, 6, 4);

    graph.addEdge(2, 3, 3);
    graph.addEdge(2, 4, 1);

    graph.addEdge(3, 1, 2);
    graph.addEdge(3, 5, 9);
    graph.addEdge(3, 6, 7);

    graph.addEdge(4, 3, 1);
    graph.addEdge(4, 7, 6);

    graph.addEdge(5, 7, 5);
    graph.addEdge(5, 8, 1);

    graph.addEdge(6, 5, 1);
    graph.addEdge(6, 8, 5);

    graph.addEdge(7, 8, 5);

    std::vector<int> dist = graph.dijkstra(0);
    assert(dist[1] == 6);
    assert(dist[2] == 2);
    assert(dist[3] == 4);
    assert(dist[4] == 3);
    assert(dist[5] == 11);
    assert(dist[6] == 10);
    assert(dist[7] == 9);
    assert(dist[8] == 12);
}

void testRequest()
{
    {
        std::stringstream in;
        in << "6\n";
        in << "9\n";
        in << "0 3 1\n";
        in << "0 4 2\n";
        in << "1 2 7\n";
        in << "1 3 2\n";
        in << "1 4 3\n";
        in << "1 5 3\n";
        in << "2 5 3\n";
        in << "3 4 4\n";
        in << "3 5 6\n";
        in << "0 2";
        std::stringstream out;
        processRequest(in, out);
        assert(out.str() == "9\n");
    }
}

void testAll()
{
    testGraph();
    testDijkstra();
    testRequest();
    std::cout << "ALL TESTS PASSED\n";
}

int main()
{
    testAll();
    //processRequest(std::cin, std::cout);
    return 0;
}