#include "stdafx.h"
#include "CppUnitTest.h"
#include "../CPPace.Lib/LexBFS.h"
#include "../CPPace.Lib/Kernelizer.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace CPPaceTests {
  TEST_CLASS(KernelizerTests) {
private: 
    /// copied from lexbfs chordality check, to help MCS order test
  static bool is_chordal_given_order(AdjacencyList& graph, map<int, int> order)
  {
      for (int v = 0; v < order.size(); v++) {
        int ordering = order[v];
        set<int> higher_order_neighbours;
        for (int n : graph.edges(v)) {
          if (order.find(n) != order.end() && order[n] > ordering) {
            higher_order_neighbours.emplace(n);
          }
        }
        
        if (!graph.is_clique(higher_order_neighbours)) {
          return false;
        }
      }
      return true;
  }
public:

    TEST_METHOD(MCS) {
      AdjacencyList graph = SampleGraphs::berry_bordat();
      map<int, int> order = Kernelizer::MCS(graph);
      Assert::IsFalse(is_chordal_given_order(graph, order));

      LexBFS lex(graph.num_vertices);
      lex.order(graph);
      Assert::AreEqual(lex.is_chordal(graph), is_chordal_given_order(graph, order));

      graph = AdjacencyList(5);
      order = Kernelizer::MCS(graph);
      Assert::IsTrue(is_chordal_given_order(graph, order));
      
      graph.add_edge(0, 1);
      order = Kernelizer::MCS(graph);
      Assert::IsTrue(is_chordal_given_order(graph, order));

      graph.add_edge(1, 2);
      order = Kernelizer::MCS(graph);
      Assert::IsTrue(is_chordal_given_order(graph, order));

      graph.add_edge(2, 3);
      order = Kernelizer::MCS(graph);
      Assert::IsTrue(is_chordal_given_order(graph, order));

      graph.add_edge(0, 3);
      order = Kernelizer::MCS(graph);
      Assert::IsFalse(is_chordal_given_order(graph, order));

      graph.add_edge(0, 2);
      order = Kernelizer::MCS(graph);
      Assert::IsTrue(is_chordal_given_order(graph, order));

      graph.add_edge(3, 4);
      order = Kernelizer::MCS(graph);
      Assert::IsTrue(is_chordal_given_order(graph, order));

      graph.add_edge(1, 4);
      order = Kernelizer::MCS(graph);
      Assert::IsFalse(is_chordal_given_order(graph, order));

      graph = AdjacencyList(5);
      graph.add_edge(0, 1);
      graph.add_edge(1, 2);
      graph.add_edge(2, 3);
      graph.add_edge(3, 4);
      graph.add_edge(0, 3);
      graph.add_edge(0, 4);
      order = Kernelizer::MCS(graph);
      Assert::IsFalse(is_chordal_given_order(graph, order));

      graph.remove_vertex(4);
      order = Kernelizer::MCS(graph);
      Assert::IsFalse(is_chordal_given_order(graph, order));

      graph = AdjacencyList(5);
      graph.add_edge(0, 1);
      graph.add_edge(1, 2);
      graph.add_edge(2, 3);
      graph.add_edge(3, 4);
      graph.add_edge(0, 3);
      graph.add_edge(0, 4);
      graph.remove_vertex(3);
      order = Kernelizer::MCS(graph);
      Assert::IsTrue(is_chordal_given_order(graph, order));

      graph = AdjacencyList(5);
      graph.add_edge(0, 1);
      graph.add_edge(1, 2);
      graph.add_edge(2, 3);
      graph.add_edge(3, 4);
      graph.add_edge(0, 3);
      graph.add_edge(0, 4);
      graph.remove_edge(0, 3);
      order = Kernelizer::MCS(graph);
      Assert::IsFalse(is_chordal_given_order(graph, order));

      graph = SampleGraphs::multiple_moplexes_and_big_cc();
      order = Kernelizer::MCS(graph);
      Assert::IsFalse(is_chordal_given_order(graph, order));

      graph.remove_vertex(8);
      order = Kernelizer::MCS(graph);
      Assert::IsTrue(is_chordal_given_order(graph, order));

      graph = SampleGraphs::multiple_moplexes_and_big_cc();
      graph.remove_vertex(4);
      order = Kernelizer::MCS(graph);
      Assert::IsTrue(is_chordal_given_order(graph, order));

      graph.add_edge(2, 5);
      order = Kernelizer::MCS(graph);
      Assert::IsFalse(is_chordal_given_order(graph, order));
    }

    TEST_METHOD(FindChordlessCycle) {
      AdjacencyList graph(5);
      vector<int> cycle;
      Assert::IsFalse(Kernelizer::find_chordless_cycle(graph, cycle));

      graph.add_edge(0, 1);
      Assert::IsFalse(Kernelizer::find_chordless_cycle(graph, cycle));

      graph.add_edge(1, 2);
      Assert::IsFalse(Kernelizer::find_chordless_cycle(graph, cycle));

      graph.add_edge(2, 3);
      Assert::IsFalse(Kernelizer::find_chordless_cycle(graph, cycle));

      graph.add_edge(3, 4);
      Assert::IsFalse(Kernelizer::find_chordless_cycle(graph, cycle));

      cycle = vector<int>();
      graph.add_edge(0, 3);
      Assert::IsTrue(Kernelizer::find_chordless_cycle(graph, cycle));
      Assert::AreEqual(4, (int)cycle.size());
      set<int> uniqueTest;
      for(int v : cycle) {
        uniqueTest.emplace(v);
      }
      Assert::AreEqual(uniqueTest.size(), cycle.size());      

      cycle = vector<int>();
      graph.add_edge(0, 4);
      graph.remove_edge(0, 3);
      Assert::IsTrue(Kernelizer::find_chordless_cycle(graph, cycle));
      Assert::AreEqual(5, (int)cycle.size());
      uniqueTest = set<int>();
      for (int v : cycle) {
        uniqueTest.emplace(v);
      }
      Assert::AreEqual(uniqueTest.size(), cycle.size());

      graph = SampleGraphs::multiple_moplexes_and_big_cc();
      cycle = vector<int>();
      Assert::IsTrue(Kernelizer::find_chordless_cycle(graph, cycle));
      Assert::AreEqual(6, (int)cycle.size());
      uniqueTest = set<int>();
      for (int v : cycle) {
        uniqueTest.emplace(v);
      }
      Assert::AreEqual(uniqueTest.size(), cycle.size());

      graph = SampleGraphs::berry_bordat();
      cycle = vector<int>();
      Assert::IsTrue(Kernelizer::find_chordless_cycle(graph, cycle));
      Assert::AreEqual(4, (int)cycle.size());
      uniqueTest = set<int>();
      for (int v : cycle) {
        uniqueTest.emplace(v);
      }
      Assert::AreEqual(uniqueTest.size(), cycle.size());

      graph.remove_vertex(3);
      cycle = vector<int>();
      Assert::IsTrue(Kernelizer::find_chordless_cycle(graph, cycle));
      uniqueTest = set<int>();
      for (int v : cycle) {
        uniqueTest.emplace(v);
      }
      Assert::AreEqual(uniqueTest.size(), cycle.size());

      graph = SampleGraphs::multiple_moplexes_and_big_cc();
      graph.remove_vertex(4);
      cycle = vector<int>();
      Assert::IsFalse(Kernelizer::find_chordless_cycle(graph, cycle));

      graph.add_edge(2, 5);
      cycle = vector<int>();
      Assert::IsTrue(Kernelizer::find_chordless_cycle(graph, cycle));
      uniqueTest = set<int>();
      for (int v : cycle) {
        uniqueTest.emplace(v);
      }
      Assert::AreEqual(uniqueTest.size(), cycle.size());
    }

    TEST_METHOD(BFSPath) {
      AdjacencyList graph(5);
      vector<int> path;
      Assert::IsFalse(Kernelizer::BFS_path(graph, 0, 1, path));

      graph.add_edge(0, 1);
      Assert::IsTrue(Kernelizer::BFS_path(graph, 0, 1, path));
      Assert::AreEqual(2, (int)path.size());

      path = vector<int>();
      graph.add_edge(1, 2);
      Assert::IsTrue(Kernelizer::BFS_path(graph, 0, 2, path));
      Assert::AreEqual(3, (int)path.size());

      path = vector<int>();
      graph.add_edge(2, 3);
      Assert::IsTrue(Kernelizer::BFS_path(graph, 0, 3, path));
      Assert::AreEqual(4, (int)path.size());

      path = vector<int>();
      graph.add_edge(3, 4);
      Assert::IsTrue(Kernelizer::BFS_path(graph, 0, 4, path));
      Assert::AreEqual(5, (int)path.size());

      path = vector<int>();
      Assert::IsTrue(Kernelizer::BFS_path(graph, 2, 4, path));
      Assert::AreEqual(3, (int)path.size());

      path = vector<int>();
      graph.add_edge(0, 3);
      Assert::IsTrue(Kernelizer::BFS_path(graph, 0, 4, path));
      Assert::AreEqual(3, (int)path.size());

      path = vector<int>();
      graph.remove_vertex(0);
      Assert::IsTrue(Kernelizer::BFS_path(graph, 1, 4, path));
      Assert::AreEqual(4, (int)path.size());

      path = vector<int>();
      graph.remove_vertex(3);
      Assert::IsFalse(Kernelizer::BFS_path(graph, 1, 4, path));

      graph = AdjacencyList(5);
      graph.add_edge(0, 1);
      graph.add_edge(1, 2);
      graph.add_edge(2, 3);
      graph.add_edge(3, 4);
      graph.add_edge(0, 3);
      graph.add_edge(0, 4);
      graph.remove_edge(0, 3);
      graph.remove_vertex(4);
      path = vector<int>();
      Assert::IsTrue(Kernelizer::BFS_path(graph, 0, 3, path));
      Assert::AreEqual(4, (int)path.size());

      path = vector<int>();
      Assert::IsTrue(Kernelizer::BFS_path(graph, 3, 0, path));
      Assert::AreEqual(4, (int)path.size());
    }

    TEST_METHOD(Phase1) {
      AdjacencyList graph(5);
      Kernel kernel = Kernelizer::phase1(graph);
      Assert::AreEqual(0, (int)kernel.a.size());
      Assert::AreEqual(5, (int)kernel.b.size());

      graph.add_edge(0, 1);
      kernel = Kernelizer::phase1(graph);
      Assert::AreEqual(0, (int)kernel.a.size());
      Assert::AreEqual(5, (int)kernel.b.size());

      graph.add_edge(1, 2);
      kernel = Kernelizer::phase1(graph);
      Assert::AreEqual(0, (int)kernel.a.size());
      Assert::AreEqual(5, (int)kernel.b.size());

      graph.add_edge(2, 3);
      kernel = Kernelizer::phase1(graph);
      Assert::AreEqual(0, (int)kernel.a.size());
      Assert::AreEqual(5, (int)kernel.b.size());

      graph.add_edge(0, 3);
      kernel = Kernelizer::phase1(graph);
      Assert::AreEqual(4, (int)kernel.a.size());
      Assert::AreEqual(1, (int)kernel.b.size());
      Assert::IsTrue(kernel.b.find(4) != kernel.b.end());

      graph = SampleGraphs::berry_bordat();
      kernel = Kernelizer::phase1(graph);
      Assert::AreEqual(4, (int)kernel.a.size());
      Assert::AreEqual(4, (int)kernel.b.size());

      graph = SampleGraphs::multiple_moplexes_and_big_cc();
      kernel = Kernelizer::phase1(graph);
      Assert::AreEqual(6, (int)kernel.a.size());
      Assert::AreEqual(5, (int)kernel.b.size());
    }
  };
}