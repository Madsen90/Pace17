#include "stdafx.h"
#include "CppUnitTest.h"
#include "../CPPace.Lib/MinimumFillIn.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace CPPaceTests {
  TEST_CLASS(MinimumFillInTests) {
  public:
    TEST_METHOD(ChordlessPath) {
      AdjacencyList graph(5);
      Assert::IsTrue(MinimumFillIn::is_path_chordless(graph, vector<int> { 0 }));

      // 0-1 2 3 4
      graph.add_edge(0, 1);
      Assert::IsTrue(MinimumFillIn::is_path_chordless(graph, vector<int> { 0 }));
      Assert::IsTrue(MinimumFillIn::is_path_chordless(graph, vector<int> { 0, 1 }));

      // 0-1-2-3-4
      graph.add_edge(1, 2);
      graph.add_edge(3, 4);
      graph.add_edge(2, 3);
      Assert::IsTrue(MinimumFillIn::is_path_chordless(graph, vector<int> { 0, 1, 2, 3, 4 }));
      Assert::IsTrue(MinimumFillIn::is_path_chordless(graph, vector<int> { 4, 3, 2, 1, 0 }));

      // 0   2-3-4
      //  \_/
      graph.add_edge(0, 2);
      Assert::IsFalse(MinimumFillIn::is_path_chordless(graph, vector<int> { 0, 1, 2, 3, 4 }));
      Assert::IsFalse(MinimumFillIn::is_path_chordless(graph, vector<int> { 4, 3, 2, 1, 0 }));
      Assert::IsTrue(MinimumFillIn::is_path_chordless(graph, vector<int> { 0, 2, 3, 4 }));
      Assert::IsTrue(MinimumFillIn::is_path_chordless(graph, vector<int> { 4, 3, 2, 0 }));

      // 0   2-3-4
      //  \_/ \_/
      graph.add_edge(2, 4);
      Assert::IsFalse(MinimumFillIn::is_path_chordless(graph, vector<int> { 0, 2, 3, 4 }));
      Assert::IsFalse(MinimumFillIn::is_path_chordless(graph, vector<int> { 4, 3, 2, 0 }));
    }

    TEST_METHOD(FindFourCycle) {
      AdjacencyList graph(5);
      vector<int> cycle;
      Assert::IsFalse(MinimumFillIn::find_four_cycle(graph, cycle));

      graph.add_edge(0, 1);
      Assert::IsFalse(MinimumFillIn::find_four_cycle(graph, cycle));
      Assert::AreEqual(0, (int)cycle.size());

      graph.add_edge(1, 2);
      Assert::IsFalse(MinimumFillIn::find_four_cycle(graph, cycle));
      Assert::AreEqual(0, (int)cycle.size());

      graph.add_edge(2, 3);
      Assert::IsFalse(MinimumFillIn::find_four_cycle(graph, cycle));
      Assert::AreEqual(0, (int)cycle.size());

      graph.add_edge(3, 4);
      Assert::IsFalse(MinimumFillIn::find_four_cycle(graph, cycle));
      Assert::AreEqual(0, (int)cycle.size());

      graph.add_edge(4, 0);
      Assert::IsFalse(MinimumFillIn::find_four_cycle(graph, cycle));
      Assert::AreEqual(0, (int)cycle.size());

      graph.add_edge(3, 0);
      Assert::IsTrue(MinimumFillIn::find_four_cycle(graph, cycle));
      Assert::AreEqual(4, (int)cycle.size());

      graph.remove_vertex(0);
      Assert::IsFalse(MinimumFillIn::find_four_cycle(graph, cycle));
      Assert::AreEqual(0, (int)cycle.size());

      graph.add_edge(1, 4);
      Assert::IsTrue(MinimumFillIn::find_four_cycle(graph, cycle));
      Assert::AreEqual(4, (int)cycle.size());

      graph.add_vertex(0);
      graph.remove_edge(1, 4);
      Assert::IsTrue(MinimumFillIn::find_four_cycle(graph, cycle));
      Assert::AreEqual(4, (int)cycle.size());
    }

    TEST_METHOD(FindFourCycleBerryBordat) {
      AdjacencyList graph = SampleGraphs::berry_bordat();
      vector<int> cycle;
      Assert::IsTrue(MinimumFillIn::find_four_cycle(graph, cycle));
      Assert::AreEqual(4, (int)cycle.size());

      graph.remove_vertex(5);
      Assert::IsFalse(MinimumFillIn::find_four_cycle(graph, cycle));
      Assert::AreEqual(0, (int)cycle.size());
    }

    TEST_METHOD(FindVStar) {
      int v_star;
      AdjacencyList graph(5);
      Assert::IsFalse(MinimumFillIn::find_v_star(graph, 0, 2, set<int>(), v_star));

      graph.add_edge(0, 1);
      Assert::IsFalse(MinimumFillIn::find_v_star(graph, 0, 2, set<int>(), v_star));

      graph.add_edge(1, 2);
      Assert::IsTrue(MinimumFillIn::find_v_star(graph, 0, 2, set<int>(), v_star));
      Assert::AreEqual(1, v_star);

      graph.add_edge(2, 3);
      Assert::IsTrue(MinimumFillIn::find_v_star(graph, 0, 3, set<int>(), v_star));
      Assert::IsTrue(v_star == 1 || v_star == 2);

      graph.remove_vertex(2);
      Assert::IsFalse(MinimumFillIn::find_v_star(graph, 0, 2, set<int>(), v_star));
      Assert::IsFalse(MinimumFillIn::find_v_star(graph, 0, 3, set<int>(), v_star));

      graph.add_edge(1, 4);
      graph.add_edge(3, 4);
      Assert::IsTrue(MinimumFillIn::find_v_star(graph, 0, 3, set<int>(), v_star));
      Assert::IsTrue(v_star == 4 || v_star == 1);

      Assert::IsTrue(MinimumFillIn::find_v_star(graph, 0, 3, set<int>(), v_star));
      Assert::IsTrue(v_star == 4 || v_star == 1);

      graph.add_vertex(2);

      Assert::IsTrue(MinimumFillIn::find_v_star(graph, 0, 3, set<int>(), v_star));
      Assert::AreEqual(1, v_star);

      graph.add_edge(0, 4);
      Assert::IsFalse(MinimumFillIn::find_v_star(graph, 0, 3, set<int>(), v_star));

      Assert::IsTrue(MinimumFillIn::find_v_star(graph, 0, 3, set<int> { 4 }, v_star));
      Assert::IsTrue(v_star == 1 || v_star == 2);
    }

    TEST_METHOD(FindMoplexesBerryBordat) {
      AdjacencyList graph = SampleGraphs::berry_bordat();
      vector<set<int>> moplexes = MinimumFillIn::find_moplexes(graph);
      Assert::IsTrue(vector<set<int>> {
        set<int> { 0, 1 },
        set<int> { 2 },
        set<int> { 3 },
        set<int> { 4 },
        set<int> { 6 },
        set<int> { 7 }
      } == moplexes); 
    }

    TEST_METHOD(FindMinFillBerryBordat) {
      AdjacencyList graph = SampleGraphs::berry_bordat();
      stack<pair<int, int>> edges = MinimumFillIn::minimum_fill_in(graph);
      Assert::AreEqual(2, (int)edges.size());
      Assert::IsTrue(edges.top().first == 4 && edges.top().second == 5 || edges.top().first == 5 && edges.top().second == 6);
      edges.pop();
      Assert::IsTrue(edges.top().first == 4 && edges.top().second == 5 || edges.top().first == 5 && edges.top().second == 6);
    }

    TEST_METHOD(MinimumFillInKTests) {
      //4-cycle
      AdjacencyList graph = AdjacencyList(4);
      graph.add_edge(0, 1);
      graph.add_edge(1, 2);
      graph.add_edge(2, 3);
      graph.add_edge(3, 0);

      stack<pair<int, int>> result = MinimumFillIn::minimum_fill_in(graph);
      Assert::IsTrue(1 == result.size());

      //5-cycle
      graph = AdjacencyList(5);
      graph.add_edge(0, 1);
      graph.add_edge(1, 2);
      graph.add_edge(2, 3);
      graph.add_edge(3, 4);
      graph.add_edge(4, 0);

      result = MinimumFillIn::minimum_fill_in(graph);
      Assert::IsTrue(2 == result.size());      

      //2 single nodes
      graph = AdjacencyList(2);

      result = MinimumFillIn::minimum_fill_in(graph);
      Assert::IsTrue(0 == result.size());

      //2 4-cycles
      graph = AdjacencyList(8);
      graph.add_edge(0, 1);
      graph.add_edge(1, 2);
      graph.add_edge(2, 3);
      graph.add_edge(3, 0);

      graph.add_edge(4, 5);
      graph.add_edge(5, 6);
      graph.add_edge(6, 7);
      graph.add_edge(7, 4);

      result = MinimumFillIn::minimum_fill_in(graph);
      Assert::IsTrue(2 == result.size());

      //10 cycle
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
      graph.add_edge(9, 0);

      result = MinimumFillIn::minimum_fill_in(graph);
      Assert::IsTrue(7 == result.size());

      //2 combined 4-cycles. Solvable with one.
      graph = AdjacencyList(6);
      graph.add_edge(0, 1);
      graph.add_edge(1, 4);
      graph.add_edge(0, 2);
      graph.add_edge(0, 3);
      graph.add_edge(3, 4);

      result = MinimumFillIn::minimum_fill_in(graph);
      Assert::IsTrue(1 == result.size());

      //10 combined 4-cycles. Solvable with one
      graph = AdjacencyList(12);
      graph.add_edge(0, 1);
      graph.add_edge(0, 2);
      graph.add_edge(0, 3);
      graph.add_edge(0, 5);
      graph.add_edge(0, 6);
      graph.add_edge(0, 7);
      graph.add_edge(0, 8);
      graph.add_edge(0, 9);
      graph.add_edge(0, 10);
      graph.add_edge(0, 11);
      graph.add_edge(4, 1);
      graph.add_edge(4, 2);
      graph.add_edge(4, 3);
      graph.add_edge(4, 5);
      graph.add_edge(4, 6);
      graph.add_edge(4, 7);
      graph.add_edge(4, 8);
      graph.add_edge(4, 9);
      graph.add_edge(4, 10);
      graph.add_edge(4, 11);

      result = MinimumFillIn::minimum_fill_in(graph);
      Assert::IsTrue(1 == result.size());
    }
  };
}
