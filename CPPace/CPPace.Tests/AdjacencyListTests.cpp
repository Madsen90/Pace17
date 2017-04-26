#include "stdafx.h"
#include "CppUnitTest.h"
#include "../CPPace.Lib/AdjacencyList.h"

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
      AdjacencyList graph(5);
      
      // First component
      graph.add_edge(0, 1);
      graph.add_edge(1, 2);
      graph.add_edge(2, 0);

      // Second component
      graph.add_edge(3, 4);

      Assert::IsTrue(graph.connected(0, 1));
      Assert::IsTrue(graph.connected(1, 0));
      Assert::IsTrue(graph.connected(0, 2));
      Assert::IsTrue(graph.connected(3, 4));

      Assert::IsFalse(graph.connected(0, 4));
      Assert::IsFalse(graph.connected(3, 2));
    }

    TEST_METHOD(BerryBordatGraph) {
      AdjacencyList graph(8);
      graph.add_edge(0, 1);
      graph.add_edge(0, 2);
      graph.add_edge(0, 4);
      graph.add_edge(0, 5);
      graph.add_edge(1, 2);
      graph.add_edge(1, 4);
      graph.add_edge(1, 5);
      graph.add_edge(2, 5);
      graph.add_edge(2, 6);
      graph.add_edge(3, 5);
      graph.add_edge(3, 6);
      graph.add_edge(4, 7);
      graph.add_edge(5, 7);
      graph.add_edge(6, 7);

      Assert::AreEqual(14, graph.num_edges);
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
