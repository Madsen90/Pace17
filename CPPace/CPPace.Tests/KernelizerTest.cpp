#include "stdafx.h"
#include "CppUnitTest.h"
#include "../CPPace.Lib/LexBFS.h"
#include "../CPPace.Lib/Kernelizer.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace CPPaceTests {
  TEST_CLASS(KernelizerTests) {
public:

    TEST_METHOD(MCS) {
      AdjacencyList graph = SampleGraphs::berry_bordat();
      map<int, int> order = Kernelizer::MCS(graph);
      Assert::IsFalse(Kernelizer::is_chordal_given_order(graph, order));

      LexBFS lex(graph.num_vertices);
      lex.order(graph);
      Assert::AreEqual(lex.is_chordal(graph), Kernelizer::is_chordal_given_order(graph, order));

      graph = AdjacencyList(5);
      order = Kernelizer::MCS(graph);
      Assert::IsTrue(Kernelizer::is_chordal_given_order(graph, order));
      
      graph.add_edge(0, 1);
      order = Kernelizer::MCS(graph);
      Assert::IsTrue(Kernelizer::is_chordal_given_order(graph, order));

      graph.add_edge(1, 2);
      order = Kernelizer::MCS(graph);
      Assert::IsTrue(Kernelizer::is_chordal_given_order(graph, order));

      graph.add_edge(2, 3);
      order = Kernelizer::MCS(graph);
      Assert::IsTrue(Kernelizer::is_chordal_given_order(graph, order));

      graph.add_edge(0, 3);
      order = Kernelizer::MCS(graph);
      Assert::IsFalse(Kernelizer::is_chordal_given_order(graph, order));

      graph.add_edge(0, 2);
      order = Kernelizer::MCS(graph);
      Assert::IsTrue(Kernelizer::is_chordal_given_order(graph, order));

      graph.add_edge(3, 4);
      order = Kernelizer::MCS(graph);
      Assert::IsTrue(Kernelizer::is_chordal_given_order(graph, order));

      graph.add_edge(1, 4);
      order = Kernelizer::MCS(graph);
      Assert::IsFalse(Kernelizer::is_chordal_given_order(graph, order));

      graph = AdjacencyList(5);
      graph.add_edge(0, 1);
      graph.add_edge(1, 2);
      graph.add_edge(2, 3);
      graph.add_edge(3, 4);
      graph.add_edge(0, 3);
      graph.add_edge(0, 4);
      order = Kernelizer::MCS(graph);
      Assert::IsFalse(Kernelizer::is_chordal_given_order(graph, order));

      graph.remove_vertex(4);
      order = Kernelizer::MCS(graph);
      Assert::IsFalse(Kernelizer::is_chordal_given_order(graph, order));

      graph = AdjacencyList(5);
      graph.add_edge(0, 1);
      graph.add_edge(1, 2);
      graph.add_edge(2, 3);
      graph.add_edge(3, 4);
      graph.add_edge(0, 3);
      graph.add_edge(0, 4);
      graph.remove_vertex(3);
      order = Kernelizer::MCS(graph);
      Assert::IsTrue(Kernelizer::is_chordal_given_order(graph, order));

      graph = AdjacencyList(5);
      graph.add_edge(0, 1);
      graph.add_edge(1, 2);
      graph.add_edge(2, 3);
      graph.add_edge(3, 4);
      graph.add_edge(0, 3);
      graph.add_edge(0, 4);
      graph.remove_edge(0, 3);
      order = Kernelizer::MCS(graph);
      Assert::IsFalse(Kernelizer::is_chordal_given_order(graph, order));

      graph = SampleGraphs::multiple_moplexes_and_big_cc();
      order = Kernelizer::MCS(graph);
      Assert::IsFalse(Kernelizer::is_chordal_given_order(graph, order));

      graph.remove_vertex(8);
      order = Kernelizer::MCS(graph);
      Assert::IsTrue(Kernelizer::is_chordal_given_order(graph, order));

      graph = SampleGraphs::multiple_moplexes_and_big_cc();
      graph.remove_vertex(4);
      order = Kernelizer::MCS(graph);
      Assert::IsTrue(Kernelizer::is_chordal_given_order(graph, order));

      graph.add_edge(2, 5);
      order = Kernelizer::MCS(graph);
      Assert::IsFalse(Kernelizer::is_chordal_given_order(graph, order));
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

      // cube (each side is a four cycle)
      graph = AdjacencyList(8);
      graph.add_edge(0, 1);
      graph.add_edge(0, 2);
      graph.add_edge(1, 3);
      graph.add_edge(2, 3);

      graph.add_edge(0, 4);
      graph.add_edge(1, 5);
      graph.add_edge(2, 6);
      graph.add_edge(3, 7);

      graph.add_edge(4, 5);
      graph.add_edge(4, 6);
      graph.add_edge(5, 7);
      graph.add_edge(6, 7);
      kernel = Kernelizer::phase1(graph);
      Assert::AreEqual(8, (int)kernel.a.size());
      Assert::AreEqual(0, (int)kernel.b.size());

      //could probably use more testing

    }

    TEST_METHOD(Phase2) {
      AdjacencyList graph = SampleGraphs::multiple_moplexes_and_big_cc();
      Kernel kernel = Kernelizer::phase1(graph);
      Assert::AreEqual(6, (int)kernel.a.size());
      Assert::AreEqual(5, (int)kernel.b.size());
      Assert::AreEqual(3, kernel.kMin);
      Assert::IsTrue(Kernelizer::phase2(graph, kernel));
      Assert::AreEqual(6, (int)kernel.a.size());
      Assert::AreEqual(5, (int)kernel.b.size());
      Assert::AreEqual(3, kernel.kMin);

      graph = SampleGraphs::berry_bordat();
      kernel = Kernelizer::phase1(graph);
      Assert::AreEqual(4, (int)kernel.a.size());
      Assert::AreEqual(4, (int)kernel.b.size());
      Assert::AreEqual(1, kernel.kMin);
      Assert::IsTrue(Kernelizer::phase2(graph, kernel));
      Assert::AreEqual(7, (int)kernel.a.size());
      Assert::AreEqual(1, (int)kernel.b.size());
      Assert::AreEqual(2, kernel.kMin);

      //false kernel check;
      graph = SampleGraphs::berry_bordat();
      Kernel kernel2;
      kernel2.a = set<int>{ 0, 4, 7, 2, 5, 3, 6};
      kernel2.b = set<int>{ 1 };
      kernel2.kMin = 2; //I think?
      Assert::IsTrue(Kernelizer::phase2(graph, kernel2));
      Assert::AreEqual(kernel.a.size(), kernel2.a.size());
      Assert::AreEqual(kernel.b.size(), kernel2.b.size());
      Assert::AreEqual(2, kernel2.kMin);

      // two four cycles sharing one edge
      graph = AdjacencyList(6);
      graph.add_edge(0, 1);
      graph.add_edge(0, 2);
      graph.add_edge(1, 3);
      graph.add_edge(2, 3);
      graph.add_edge(2, 4);
      graph.add_edge(3, 5);
      graph.add_edge(4, 5);
      kernel = Kernelizer::phase1(graph);
      Assert::AreEqual(4, (int)kernel.a.size());
      Assert::AreEqual(2, (int)kernel.b.size());
      Assert::AreEqual(1, kernel.kMin);
      Assert::IsTrue(Kernelizer::phase2(graph, kernel));
      Assert::AreEqual(6, (int)kernel.a.size());
      Assert::AreEqual(0, (int)kernel.b.size());
      Assert::AreEqual(2, kernel.kMin);

      // three four cycles sharing one edge
      graph = AdjacencyList(8);
      graph.add_edge(0, 1);
      graph.add_edge(0, 2);
      graph.add_edge(1, 3);
      graph.add_edge(2, 3);
      graph.add_edge(2, 4);
      graph.add_edge(3, 5);
      graph.add_edge(4, 5);
      graph.add_edge(2, 6);
      graph.add_edge(3, 7);
      graph.add_edge(6, 7);
      kernel = Kernelizer::phase1(graph);
      Assert::AreEqual(4, (int)kernel.a.size());
      Assert::AreEqual(4, (int)kernel.b.size());
      Assert::AreEqual(1, kernel.kMin);
      Assert::IsTrue(Kernelizer::phase2(graph, kernel));
      Assert::AreEqual(8, (int)kernel.a.size());
      Assert::AreEqual(0, (int)kernel.b.size());
      Assert::AreEqual(3, kernel.kMin);

     //five cycle, with four cycle connected on 1 edge and another 5 cycle on another edge.
      graph = AdjacencyList(10);
      graph.add_edge(0, 1);
      graph.add_edge(1, 2);
      graph.add_edge(2, 3);
      graph.add_edge(3, 4);
      graph.add_edge(0, 4);

      graph.add_edge(0, 5);
      graph.add_edge(1, 6);
      graph.add_edge(5, 6);

      graph.add_edge(1, 9);
      graph.add_edge(2, 7);
      graph.add_edge(7, 8);
      graph.add_edge(8, 9);
      kernel = Kernelizer::phase1(graph);
      Assert::AreEqual(5, (int)kernel.a.size());
      Assert::AreEqual(5, (int)kernel.b.size());
      Assert::AreEqual(2, kernel.kMin);
      Assert::IsTrue(Kernelizer::phase2(graph, kernel));
      Assert::AreEqual(10, (int)kernel.a.size());
      Assert::AreEqual(0, (int)kernel.b.size());
      Assert::AreEqual(4, kernel.kMin);

      //chordless graph
      graph = AdjacencyList(10);
      graph.add_edge(0, 1);
      graph.add_edge(1, 2);
      graph.add_edge(2, 3);
      graph.add_edge(3, 4);
      graph.add_edge(4, 5);
      graph.add_edge(5, 6);
      graph.add_edge(6, 7);
      graph.add_edge(7, 8);
      graph.add_edge(8, 9);
      kernel = Kernelizer::phase1(graph);
      Assert::AreEqual(0, (int)kernel.a.size());
      Assert::AreEqual(10, (int)kernel.b.size());
      Assert::AreEqual(0, kernel.kMin);
      Assert::IsFalse(Kernelizer::phase2(graph, kernel));
      Assert::AreEqual(0, (int)kernel.a.size());
      Assert::AreEqual(10, (int)kernel.b.size());
      Assert::AreEqual(0, kernel.kMin);
    }

    TEST_METHOD(Phase3) {
      //two four cycles that share two edges
      AdjacencyList graph(5);
      graph.add_edge(0, 1);
      graph.add_edge(0, 2);
      graph.add_edge(0, 4);
      graph.add_edge(1, 3);
      graph.add_edge(2, 3);
      graph.add_edge(3, 4);
      Kernel kernel = Kernelizer::phase1(graph);
      Assert::AreEqual(4, (int)kernel.a.size());
      Assert::AreEqual(1, (int)kernel.b.size());
      Assert::AreEqual(1, kernel.kMin);
      Assert::IsTrue(Kernelizer::phase2(graph, kernel));
      Assert::AreEqual(4, (int)kernel.a.size());
      Assert::AreEqual(1, (int)kernel.b.size());
      Assert::AreEqual(1, kernel.kMin);
      Kernel phase3_kernel;
      Assert::IsFalse(Kernelizer::phase3(graph, kernel, phase3_kernel, 0));
      phase3_kernel = Kernel();
      Assert::IsTrue(Kernelizer::phase3(graph, kernel, phase3_kernel, 1));
      Assert::AreEqual(5, (int)phase3_kernel.a.size());
      Assert::AreEqual(0, (int)phase3_kernel.b.size());
      Assert::AreEqual(1, phase3_kernel.kMin);

      // 28 four-cycles, sharing the same two nodes
      graph = AdjacencyList(10);
      graph.add_edge(0, 2);
      graph.add_edge(1, 2);
      graph.add_edge(0, 3);
      graph.add_edge(1, 3);
      graph.add_edge(0, 4);
      graph.add_edge(1, 4);
      graph.add_edge(0, 5);
      graph.add_edge(1, 5);
      graph.add_edge(0, 6);
      graph.add_edge(1, 6);
      graph.add_edge(0, 7);
      graph.add_edge(1, 7);
      graph.add_edge(0, 8);
      graph.add_edge(1, 8);
      graph.add_edge(0, 9);
      graph.add_edge(1, 9);
      kernel = Kernelizer::phase1(graph);
      Assert::AreEqual(4, (int)kernel.a.size());
      Assert::AreEqual(6, (int)kernel.b.size());
      Assert::AreEqual(1, kernel.kMin);
      Assert::IsTrue(Kernelizer::phase2(graph, kernel));
      Assert::AreEqual(4, (int)kernel.a.size());
      Assert::AreEqual(6, (int)kernel.b.size());
      Assert::AreEqual(1, kernel.kMin);
      phase3_kernel = Kernel();
      Assert::IsTrue(Kernelizer::phase3(graph, kernel, phase3_kernel, 1));
      Assert::AreEqual(4, (int)phase3_kernel.a.size());
      Assert::AreEqual(6, (int)phase3_kernel.b.size());
      Assert::AreEqual(1, phase3_kernel.kMin);
      Assert::IsTrue(pair<int, int>(0, 1) == *phase3_kernel.essential_edges.begin());
      
    }

    TEST_METHOD(NonEdges) {
      AdjacencyList graph(5);
      vector<pair<int, int>> non_edges = Kernelizer::find_non_edges(graph);
      Assert::AreEqual(10, (int)non_edges.size());
      vector<pair<int, int>> edges = graph.all_edges();
      for (pair<int, int> non_edge : non_edges) {
        for (pair<int, int> edge : edges) {
          Assert::IsFalse(non_edge.first == edge.first && non_edge.second == edge.second || non_edge.first == edge.second && non_edge.second == edge.first);
        }
      }

      graph = AdjacencyList(5);
      graph.add_edge(0, 1);
      non_edges = Kernelizer::find_non_edges(graph);
      Assert::AreEqual(9, (int)non_edges.size());
      edges = graph.all_edges();
      for (pair<int, int> non_edge : non_edges) {
        for (pair<int, int> edge : edges) {
          Assert::IsFalse(non_edge.first == edge.first && non_edge.second == edge.second || non_edge.first == edge.second && non_edge.second == edge.first);
        }
      }

    }

  };
}