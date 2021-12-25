#include <iostream>
#include <vector>
#include "assert.h"
#include "IGraph.h"
#include "ListGraph.h"
#include "MatrixGraph.h"
#include "SetGraph.h"
#include "ArcGraph.h"

template <class Graph>
void testGraphMethods()
{
    IGraph *graph = new Graph(5);

    graph->AddEdge(0, 2);
    graph->AddEdge(1, 0);
    graph->AddEdge(1, 2);
    graph->AddEdge(1, 3);
    graph->AddEdge(2, 3);
    graph->AddEdge(3, 4);
    graph->AddEdge(4, 2);
    graph->AddEdge(4, 3);

    assert(graph->VerticesCount() == 5);
    assert(graph->GetNextVertices(0) == std::vector<int>({2}));
    assert(graph->GetNextVertices(1) == std::vector<int>({0, 2, 3}));
    assert(graph->GetNextVertices(2) == std::vector<int>({3}));
    assert(graph->GetNextVertices(3) == std::vector<int>({4}));
    assert(graph->GetNextVertices(4) == std::vector<int>({2, 3}));

    assert(graph->GetPrevVertices(0) == std::vector<int>({1}));
    assert(graph->GetPrevVertices(1) == std::vector<int>());
    assert(graph->GetPrevVertices(2) == std::vector<int>({0, 1, 4}));
    assert(graph->GetPrevVertices(3) == std::vector<int>({1, 2, 4}));
    assert(graph->GetPrevVertices(4) == std::vector<int>({3}));

    delete graph;
}

template <class Graph>
void testGraphCopyCtor()
{
    IGraph *graph = new ListGraph(5);

    graph->AddEdge(0, 2);
    graph->AddEdge(1, 0);
    graph->AddEdge(1, 2);
    graph->AddEdge(1, 3);
    graph->AddEdge(2, 3);
    graph->AddEdge(3, 4);
    graph->AddEdge(4, 2);
    graph->AddEdge(4, 3);

    Graph new_graph(*graph);
    assert(new_graph.VerticesCount() == 5);
    assert(new_graph.GetNextVertices(0) == std::vector<int>({2}));
    assert(new_graph.GetNextVertices(1) == std::vector<int>({0, 2, 3}));
    assert(new_graph.GetNextVertices(2) == std::vector<int>({3}));
    assert(new_graph.GetNextVertices(3) == std::vector<int>({4}));
    assert(new_graph.GetNextVertices(4) == std::vector<int>({2, 3}));

    assert(new_graph.GetPrevVertices(0) == std::vector<int>({1}));
    assert(new_graph.GetPrevVertices(1) == std::vector<int>());
    assert(new_graph.GetPrevVertices(2) == std::vector<int>({0, 1, 4}));
    assert(new_graph.GetPrevVertices(3) == std::vector<int>({1, 2, 4}));
    assert(new_graph.GetPrevVertices(4) == std::vector<int>({3}));

    delete graph;
}

template <class Graph>
void testGraph()
{
    testGraphMethods<Graph>();
    testGraphCopyCtor<Graph>();
}

void allTests()
{
    testGraph<ListGraph>();
    testGraph<MatrixGraph>();
    testGraph<SetGraph>();
    testGraph<ArcGraph>();

    std::cout << "ALL TESTS PASSED\n";
}

int main()
{
    allTests();
    return 0;
}