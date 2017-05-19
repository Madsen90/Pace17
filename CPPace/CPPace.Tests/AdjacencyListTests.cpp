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
      AdjacencyList graph(5);
      
      // First component
      graph.add_edge(0, 1);
      graph.add_edge(1, 2);
      graph.add_edge(2, 0);

      // Second component
      graph.add_edge(3, 4);

      Assert::IsTrue(graph.is_connected(0, 1));
      Assert::IsTrue(graph.is_connected(1, 0));
      Assert::IsTrue(graph.is_connected(0, 2));
      Assert::IsTrue(graph.is_connected(3, 4));

      Assert::IsFalse(graph.is_connected(0, 4));
      Assert::IsFalse(graph.is_connected(3, 2));
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

      graph.add_edge(0, 3);
      Assert::IsTrue(graph.is_clique(set<int> { 0, 3 }));
      Assert::IsFalse(graph.is_clique(set<int> { 0, 1, 2, 3 }));

      graph.add_edge(3, 1);
      graph.add_edge(2, 3);
      Assert::IsTrue(graph.is_clique(set<int> { 0, 1, 2, 3 }));
      Assert::IsTrue(graph.is_clique(set<int> { 0, 1, 2 }));
      Assert::IsTrue(graph.is_clique(set<int> { 0, 1 }));
      Assert::IsTrue(graph.is_clique(set<int> { 0 }));
    }

    TEST_METHOD(ChordlessPath) {
      AdjacencyList graph(5);
      Assert::IsTrue(graph.is_path_chordless(vector<int> { 0 }));

      // 0-1 2 3 4
      graph.add_edge(0, 1);
      Assert::IsTrue(graph.is_path_chordless(vector<int> { 0 }));
      Assert::IsTrue(graph.is_path_chordless(vector<int> { 0, 1 }));

      // 0-1-2-3-4
      graph.add_edge(1, 2);
      graph.add_edge(3, 4);
      graph.add_edge(2, 3);
      Assert::IsTrue(graph.is_path_chordless(vector<int> { 0, 1, 2, 3, 4 }));
      Assert::IsTrue(graph.is_path_chordless(vector<int> { 4, 3, 2, 1, 0 }));
      
      // 0-1-2-3-4
      //    \_/
      graph.add_edge(1, 3);
      Assert::IsFalse(graph.is_path_chordless(vector<int> { 0, 1, 2, 3, 4 }));
      Assert::IsFalse(graph.is_path_chordless(vector<int> { 4, 3, 2, 1, 0 }));
    }

    TEST_METHOD(FindFourCycle) {
      AdjacencyList graph(5);
      vector<int> cycle;
      Assert::IsFalse(graph.find_four_cycle(cycle));

      graph.add_edge(0, 1);
      Assert::IsFalse(graph.find_four_cycle(cycle));
      Assert::AreEqual(0, (int)cycle.size());

      graph.add_edge(1, 2);
      Assert::IsFalse(graph.find_four_cycle(cycle));
      Assert::AreEqual(0, (int)cycle.size());

      graph.add_edge(2, 3);
      Assert::IsFalse(graph.find_four_cycle(cycle));
      Assert::AreEqual(0, (int)cycle.size());

      graph.add_edge(3, 4);
      Assert::IsFalse(graph.find_four_cycle(cycle));
      Assert::AreEqual(0, (int)cycle.size());

      graph.add_edge(4, 0);
      Assert::IsFalse(graph.find_four_cycle(cycle));
      Assert::AreEqual(0, (int)cycle.size());

      graph.add_edge(3, 0);
      Assert::IsTrue(graph.find_four_cycle(cycle));
      Assert::AreEqual(4, (int)cycle.size());

      graph.add_edge(0, 2);
      Assert::IsFalse(graph.find_four_cycle(cycle));
      Assert::AreEqual(0, (int)cycle.size());
    }

    TEST_METHOD(FindFourCycleBerryBordat) {
      AdjacencyList graph = SampleGraphs::berry_bordat();
      vector<int> cycle;
      Assert::IsTrue(graph.find_four_cycle(cycle));
      Assert::AreEqual(4, (int)cycle.size());
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
