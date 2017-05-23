#include "stdafx.h"
#include "CppUnitTest.h"
#include "../CPPace.Lib/AdjacencyList.h"
#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace CPPaceTests {
  TEST_CLASS(AdjacencyListTests) {
  public:
    TEST_METHOD(Constructor) {
      const int n = 5;
      AdjacencyList graph(n);
      Assert::AreEqual(n, graph.num_vertices);
      Assert::AreEqual(0, graph.num_edges);
    }

    TEST_METHOD(AddEdge) {
      AdjacencyList graph(5);
      graph.add_edge(0, 1);
      Assert::IsTrue(graph.has_edge(0, 1));
      Assert::IsTrue(graph.has_edge(1, 0));
      Assert::AreEqual(1, graph.num_edges);
    }

    TEST_METHOD(AddParallelEdges) {
      AdjacencyList graph(5);
      graph.add_edge(0, 1);
      graph.add_edge(1, 0);
      Assert::IsTrue(graph.has_edge(0, 1));
      Assert::IsTrue(graph.has_edge(1, 0));
      Assert::AreEqual(1, graph.num_edges);
    }

    TEST_METHOD(RemoveEdge) {
      AdjacencyList graph(5);
      graph.add_edge(0, 1);
      graph.remove_edge(0, 1);
      Assert::IsFalse(graph.has_edge(0, 1));
      Assert::IsFalse(graph.has_edge(1, 0));
      
      graph.add_edge(0, 1);
      graph.remove_edge(1, 0); // inverse
      Assert::IsFalse(graph.has_edge(0, 1));
      Assert::IsFalse(graph.has_edge(1, 0));
    }

    TEST_METHOD(Connectivity) {
      AdjacencyList graph(6);
      
      // First component
      graph.add_edge(0, 1);
      graph.add_edge(1, 2);
      graph.add_edge(2, 0);
      graph.add_edge(2, 3);

      // Second component
      graph.add_edge(4, 5);

      Assert::IsTrue(graph.is_connected(0, 0));
      Assert::IsTrue(graph.is_connected(0, 1));
      Assert::IsTrue(graph.is_connected(1, 0));
      Assert::IsTrue(graph.is_connected(0, 2));
      Assert::IsTrue(graph.is_connected(0, 3));
      Assert::IsTrue(graph.is_connected(4, 5));
      
      Assert::IsFalse(graph.is_connected(0, 5));
      Assert::IsFalse(graph.is_connected(4, 2));

      graph.remove_vertex(2);
      Assert::IsFalse(graph.is_connected(2, 2));
      Assert::IsTrue(graph.is_connected(0, 1));
      Assert::IsFalse(graph.is_connected(0, 2));
      Assert::IsFalse(graph.is_connected(1, 2));
      
      Assert::IsTrue(graph.is_connected(3, 3));
      Assert::IsFalse(graph.is_connected(0, 3));
      Assert::IsFalse(graph.is_connected(1, 3));
      Assert::IsFalse(graph.is_connected(2, 3));
      
      Assert::IsTrue(graph.is_connected(4, 5));
      Assert::IsFalse(graph.is_connected(0, 5));
      Assert::IsFalse(graph.is_connected(4, 2));

      graph.add_edge(3, 4);
      Assert::IsTrue(graph.is_connected(3, 4));
      Assert::IsTrue(graph.is_connected(3, 5));
      Assert::IsFalse(graph.is_connected(0, 3));
      Assert::IsFalse(graph.is_connected(0, 5));
      Assert::IsFalse(graph.is_connected(2, 5));

      graph.add_vertex(2);
      Assert::IsTrue(graph.is_connected(0, 1));
      Assert::IsTrue(graph.is_connected(0, 2));
      Assert::IsTrue(graph.is_connected(0, 3));
      Assert::IsTrue(graph.is_connected(0, 4));
      Assert::IsTrue(graph.is_connected(0, 5));
    }

    TEST_METHOD(Clique) {
      AdjacencyList graph(4);
      Assert::IsTrue(graph.is_clique(set<int> { 0 }));

      graph.add_edge(0, 1);
      Assert::IsTrue(graph.is_clique(set<int> { 0 }));
      Assert::IsTrue(graph.is_clique(set<int> { 0, 1 }));
      Assert::IsTrue(graph.is_clique(set<int> { 2 }));
      Assert::IsFalse(graph.is_clique(set<int> { 0, 2 }));

      graph.add_edge(0, 2);
      Assert::IsTrue(graph.is_clique(set<int> { 0, 2 }));
      Assert::IsFalse(graph.is_clique(set<int> { 0, 1, 2 }));

      graph.add_edge(1, 2);
      Assert::IsTrue(graph.is_clique(set<int> { 0, 1, 2 }));
      Assert::IsFalse(graph.is_clique(set<int> { 0, 1, 2, 3 }));

      graph.remove_vertex(1);
      Assert::IsFalse(graph.is_clique(set<int> { 1 }));
      Assert::IsFalse(graph.is_clique(set<int> { 0, 1, 2 }));
      Assert::IsTrue(graph.is_clique(set<int> { 0, 2 }));

      graph.add_vertex(1);
      Assert::IsTrue(graph.is_clique(set<int> { 1 }));
      Assert::IsTrue(graph.is_clique(set<int> { 0, 1, 2 }));
      Assert::IsFalse(graph.is_clique(set<int> { 0, 1, 2, 3 }));

      graph.add_edge(0, 3);
      Assert::IsTrue(graph.is_clique(set<int> { 0, 3 }));
      Assert::IsFalse(graph.is_clique(set<int> { 0, 1, 2, 3 }));

      graph.add_edge(3, 1);
      graph.add_edge(2, 3);
      Assert::IsTrue(graph.is_clique(set<int> { 0, 1, 2, 3 }));
      Assert::IsTrue(graph.is_clique(set<int> { 0, 1, 2 }));
      Assert::IsTrue(graph.is_clique(set<int> { 0, 1 }));
      Assert::IsTrue(graph.is_clique(set<int> { 0 }));

      graph.remove_vertex(2);
      Assert::IsTrue(graph.is_clique(set<int> { 0, 1, 3 }));
      Assert::IsFalse(graph.is_clique(set<int> { 0, 1, 2, 3 }));
      Assert::IsFalse(graph.is_clique(set<int> { 0, 2 }));
    }

    TEST_METHOD(MakeClique) {
      AdjacencyList graph(5);
      Assert::IsFalse(graph.is_clique(set<int> { 0, 1, 2, 3, 4 }));

      graph.make_clique(set<int> { 0, 1, 4 });
      Assert::IsTrue(graph.is_clique(set<int> { 0, 1, 4 }));
      Assert::IsFalse(graph.is_clique(set<int> { 0, 1, 2, 3, 4 }));

      graph.make_clique(set<int> { 0, 1, 2, 3, 4 });
      Assert::IsTrue(graph.is_clique(set<int> { 0, 1, 2, 3, 4 }));
      Assert::IsTrue(graph.is_clique(set<int> { 0, 1, 4 }));
    }

    TEST_METHOD(SubGraphBerryBordat) {
      AdjacencyList graph = SampleGraphs::berry_bordat();
      Assert::IsTrue(set<int> { 1, 2, 4, 5 } == graph.edges(0));

      graph.remove_vertex(0);
      Assert::IsTrue(set<int> {} == graph.edges(0));
      Assert::IsTrue(set<int> { 1, 7 } == graph.edges(4));

      graph.remove_vertices(set<int> { 4, 6 });
      Assert::IsTrue(set<int> {} == graph.edges(0));
      Assert::IsTrue(set<int> { 2, 5 } == graph.edges(1));
      Assert::IsTrue(set<int> { 1, 5 } == graph.edges(2));
      Assert::IsTrue(set<int> { 5 } == graph.edges(3));
      Assert::IsTrue(set<int> {} == graph.edges(4));
      Assert::IsTrue(set<int> { 1, 2, 3, 7 } == graph.edges(5));
      Assert::IsTrue(set<int> {} == graph.edges(6));
      Assert::IsTrue(set<int> { 5 } == graph.edges(7));

      graph.add_vertex(4);
      Assert::IsTrue(set<int> { 2, 4, 5 } == graph.edges(1));
      Assert::IsTrue(set<int> { 1, 7 } == graph.edges(4));
      Assert::IsTrue(set<int> { 4, 5 } == graph.edges(7));

      graph.add_vertices(set<int> { 0, 6 });
      Assert::IsTrue(SampleGraphs::berry_bordat().edges(0) == graph.edges(0));
      Assert::IsTrue(SampleGraphs::berry_bordat().edges(1) == graph.edges(1));
      Assert::IsTrue(SampleGraphs::berry_bordat().edges(2) == graph.edges(2));
      Assert::IsTrue(SampleGraphs::berry_bordat().edges(3) == graph.edges(3));
      Assert::IsTrue(SampleGraphs::berry_bordat().edges(4) == graph.edges(4));
      Assert::IsTrue(SampleGraphs::berry_bordat().edges(5) == graph.edges(5));
      Assert::IsTrue(SampleGraphs::berry_bordat().edges(6) == graph.edges(6));
      Assert::IsTrue(SampleGraphs::berry_bordat().edges(7) == graph.edges(7));

      graph.remove_vertices(set<int> { 1, 2, 5 });
      graph.add_all_vertices();
      Assert::IsTrue(SampleGraphs::berry_bordat().edges(0) == graph.edges(0));
      Assert::IsTrue(SampleGraphs::berry_bordat().edges(1) == graph.edges(1));
      Assert::IsTrue(SampleGraphs::berry_bordat().edges(2) == graph.edges(2));
      Assert::IsTrue(SampleGraphs::berry_bordat().edges(3) == graph.edges(3));
      Assert::IsTrue(SampleGraphs::berry_bordat().edges(4) == graph.edges(4));
      Assert::IsTrue(SampleGraphs::berry_bordat().edges(5) == graph.edges(5));
      Assert::IsTrue(SampleGraphs::berry_bordat().edges(6) == graph.edges(6));
      Assert::IsTrue(SampleGraphs::berry_bordat().edges(7) == graph.edges(7));
    }
  };

  TEST_CLASS(AdjacencyListBench) {
  public:
    TEST_METHOD(AddEdgeRandom) {
      const unsigned int seed = 0x69c384b2;
      const int num_edges = 1000000;
      const int num_vertices = 1000;
      AdjacencyList graph(num_vertices);
      srand(seed);

      for (size_t i = 0; i < num_edges; i++) {
        int u = rand() % num_vertices;
        int v = rand() % num_vertices;
        graph.add_edge(u, v);
      }
    }
  };
}
