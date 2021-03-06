#include "stdafx.h"
#include "CppUnitTest.h"
#include "../CPPace.Lib/MinimumFillIn.h"
#include "../CPPace.Lib/LexBFS.h"
#include "../CPPace.Lib/GraphGenerator.h"
#include "StdUtils.h"
#include "../CPPace.Lib/MCS.h"
#include "../CPPace.Lib/Kernelizer.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

static AdjacencyList BuildNCycle(int n) {
  //Cycle with N nodes
  AdjacencyList graph(n);
  for (int i = 0; i < n; i++) {
    if (i == n - 1)
      graph.add_edge(i, 0);
    else
      graph.add_edge(i, i + 1);
  }
  return graph;
}

static AdjacencyList BuildNFullyHalfDeleted(int n) {
  //Fully connected with N nodes
  AdjacencyList graph(n);
  for (int i = 0; i < n; i++) {
    for (int j = 1; j < n; j++) {
      graph.add_edge(i, j);
    }
  }
  srand(0);
  for (int i = 0; i < n; i++) {
    graph.vertices[i].active = (rand() % 2);
  }

  return graph;
}


static AdjacencyList BuildNHalfFull(int n) {
  //Fully connected with N nodes
  srand(0);
  AdjacencyList graph(n);
  for (int i = 0; i < n; i++) {
    for (int j = 1; j < n; j++) {
      if(rand() % 2) graph.add_edge(i, j);
    }
  }

  return graph;
}


static AdjacencyList BuildNFully(int n) {
  //Fully connected with N nodes
  AdjacencyList graph(n);
  for (int i = 0; i < n; i++) {
    for (int j = 1; j < n; j++) {
      graph.add_edge(i, j);
    }
  }
  return graph;
}

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
      vector<set<int>> moplexes = MinimumFillIn::find_moplexes(graph, vector<set<int>>(), set<pair<int, int>>());
      Assert::IsTrue(vector<set<int>> {
        set<int> { 0, 1 },
        set<int> { 2 },
        set<int> { 3 },
        set<int> { 4 },
        set<int> { 6 },
        set<int> { 7 }
      } == moplexes); 
    }

    TEST_METHOD(FindMoplexesMultipleGraph) {
      //"BAD" graph
      AdjacencyList graph = SampleGraphs::multiple_moplexes_and_big_cc();
      vector<set<int>> moplexes = MinimumFillIn::find_moplexes(graph, vector<set<int>>(), set<pair<int, int>>());
      Assert::IsTrue(vector<set<int>> {
        set<int> { 0, 1, 2 },
        set<int> { 7 },
        set<int> { 8 },
        set<int> { 9 },
        set<int> { 10 },
      } == moplexes);

      //removing 7 to make 6 a moplex
      graph.remove_vertex(7);
      moplexes = MinimumFillIn::find_moplexes(graph, vector<set<int>>(), set<pair<int, int>>());
      Assert::IsTrue(vector<set<int>> {
        set<int> { 0, 1, 2 },
        set<int> { 6 },
        set<int> { 8 },
        set<int> { 9 },
        set<int> { 10 },
      } == moplexes);

      //removing 6 to make 4 and 5 into moplexes
      graph.remove_vertex(6);
      moplexes = MinimumFillIn::find_moplexes(graph, vector<set<int>>(), set<pair<int, int>>());
      Assert::IsTrue(vector<set<int>> {
        set<int> { 0, 1, 2 },
        set<int> { 4 },
        set<int> { 5 },
        set<int> { 8 },
        set<int> { 9 },
        set<int> { 10 },
      } == moplexes);

      //readding 7 
      graph.add_vertex(7);
      moplexes = MinimumFillIn::find_moplexes(graph, vector<set<int>>(), set<pair<int, int>>());
      Assert::IsTrue(vector<set<int>> {
        set<int> { 0, 1, 2 },
          set<int> { 4 },
          set<int> { 5 },
          set<int> { 7 },
          set<int> { 8 },
          set<int> { 9 },
          set<int> { 10 },
      } == moplexes);
    }

    TEST_METHOD(FindMoplexesCycle) {
      //10-cycle
      AdjacencyList graph = GraphGenerator::cycle(10);
      vector<set<int>> moplexes = MinimumFillIn::find_moplexes(graph, vector<set<int>>(), set<pair<int, int>>());
      Assert::IsTrue(vector<set<int>> {
        set<int> { 0 },
        set<int> { 1 },
        set<int> { 2 },
        set<int> { 3 },
        set<int> { 4 },
        set<int> { 5 },
        set<int> { 6 },
        set<int> { 7 },
        set<int> { 8 },
        set<int> { 9 },
      } == moplexes);

      //straight path
      graph.remove_vertex(0);
      moplexes = MinimumFillIn::find_moplexes(graph, vector<set<int>>(), set<pair<int, int>>());
      Assert::IsTrue(vector<set<int>> {
        set<int> { 1 },
        set<int> { 9 },
      } == moplexes);

      //readding 0
      graph.add_vertex(0);
      moplexes = MinimumFillIn::find_moplexes(graph, vector<set<int>>(), set<pair<int, int>>());
      Assert::IsTrue(vector<set<int>> {
        set<int> { 0 },
        set<int> { 1 },
        set<int> { 2 },
        set<int> { 3 },
        set<int> { 4 },
        set<int> { 5 },
        set<int> { 6 },
        set<int> { 7 },
        set<int> { 8 },
        set<int> { 9 },
      } == moplexes);

      // making 0 a simplicial moplex
      graph.add_edge(1, 9);
      moplexes = MinimumFillIn::find_moplexes(graph, vector<set<int>>(), set<pair<int, int>>());
      Assert::IsTrue(vector<set<int>> {
        set<int> { 0 },
        set<int> { 2 },
        set<int> { 3 },
        set<int> { 4 },
        set<int> { 5 },
        set<int> { 6 },
        set<int> { 7 },
        set<int> { 8 },
      } == moplexes);
    }

    static bool moplex_order(set<int> a, set<int> b)
    {
      for (int i = 0; i < std_min(a.size(), b.size()); i++) {
        if(*next(a.begin(), i) == *next(b.begin(), i)) continue;
        return *next(a.begin(), i) < *next(b.begin(), i);
      }
      return a.size() < b.size();
    }

    TEST_METHOD(FindMoplexCachingCycle) {
      // initial result
      vector<set<int>> result;
      result.push_back(set<int> { 0 });
      result.push_back(set<int> { 1 });
      result.push_back(set<int> { 2 });
      result.push_back(set<int> { 3 });
      result.push_back(set<int> { 4 });
      result.push_back(set<int> { 5 });
      result.push_back(set<int> { 6 });
      result.push_back(set<int> { 7 });
      result.push_back(set<int> { 8 });
      result.push_back(set<int> { 9 });

      // initial cache
      vector<set<int>> cache;
      cache.push_back(set<int> { 0 });
      
      // initial added edges since last recompute
      set<pair<int, int>> edges;

      //10-cycle with {0} cached
      AdjacencyList graph = GraphGenerator::cycle(10);
      vector<set<int>> moplexes = MinimumFillIn::find_moplexes(graph, cache, edges);
      Assert::IsTrue( result == moplexes);

      //10-cycle with several moplexes cached, out of order
      cache.clear();
      cache.push_back(set<int> { 3 });
      cache.push_back(set<int> { 1 });
      cache.push_back(set<int> { 2 });
      cache.push_back(set<int> { 4 });
      moplexes = MinimumFillIn::find_moplexes(graph, cache, edges);
      std_sort(moplexes.begin(), moplexes.end(), moplex_order);
      Assert::IsTrue(result == moplexes);

      //10-cycle with several moplexes cached, including removed ones
      result.clear();
      result.push_back(set<int> { 0 });
      result.push_back(set<int> { 2 });
      result.push_back(set<int> { 3 });
      result.push_back(set<int> { 4 });
      result.push_back(set<int> { 5 });
      result.push_back(set<int> { 6 });
      result.push_back(set<int> { 7 });
      result.push_back(set<int> { 8 });

      cache.clear();
      cache.push_back(set<int> { 1 });
      cache.push_back(set<int> { 2 });
      cache.push_back(set<int> { 4 });

      edges.clear();
      edges.emplace(1, 9);
      graph.add_edge(1, 9);


      moplexes = MinimumFillIn::find_moplexes(graph, cache, edges);
      std_sort(moplexes.begin(), moplexes.end(), moplex_order);
      Assert::IsTrue(result == moplexes);


      //same case, but another cache
      cache.clear();
      cache.push_back(set<int> { 9 });
      cache.push_back(set<int> { 2 });
      cache.push_back(set<int> { 0 });

      moplexes = MinimumFillIn::find_moplexes(graph, cache, edges);
      std_sort(moplexes.begin(), moplexes.end(), moplex_order);
      Assert::IsTrue(result == moplexes);

      //same case, but one more added edge
      result.clear();
      result.push_back(set<int> { 0 });
      result.push_back(set<int> { 2 });
      result.push_back(set<int> { 4 });
      result.push_back(set<int> { 6 });
      result.push_back(set<int> { 7 });
      result.push_back(set<int> { 8 });

      cache.clear();
      cache.push_back(set<int> { 9 });
      cache.push_back(set<int> { 2 });
      cache.push_back(set<int> { 0 });


      edges.emplace(3, 5);
      graph.add_edge(3, 5);

      moplexes = MinimumFillIn::find_moplexes(graph, cache, edges);
      std_sort(moplexes.begin(), moplexes.end(), moplex_order);
      Assert::IsTrue(result == moplexes);
    }


    TEST_METHOD(FindMoplexCachingBerryBordat) {
      // initial result
      vector<set<int>> result;
      result.push_back(set<int> { 0, 1 });
      result.push_back(set<int> { 2 });
      result.push_back(set<int> { 3 });
      result.push_back(set<int> { 4 });
      result.push_back(set<int> { 6 });
      result.push_back(set<int> { 7 });

      // initial cache
      vector<set<int>> cache;

      // initial added edges since last recompute
      set<pair<int, int>> edges;

      //Berry Bordat not cached
      AdjacencyList graph = SampleGraphs::berry_bordat();
      vector<set<int>> moplexes = MinimumFillIn::find_moplexes(graph, cache, edges);
      Assert::IsTrue(result == moplexes);

      //one new edge, no cache
      edges.clear();
      edges.emplace(0, 1);
      moplexes = MinimumFillIn::find_moplexes(graph, cache, edges);
      std_sort(moplexes.begin(), moplexes.end(), moplex_order);
      Assert::IsTrue(result == moplexes);


      //everything cachced, no edges
      cache.clear();
      cache.push_back(set<int> { 0, 1 });
      cache.push_back(set<int> { 2 });
      cache.push_back(set<int> { 3 });
      cache.push_back(set<int> { 4 });
      cache.push_back(set<int> { 6 });
      cache.push_back(set<int> { 7 });

      edges.clear();
      moplexes = MinimumFillIn::find_moplexes(graph, cache, edges);
      std_sort(moplexes.begin(), moplexes.end(), moplex_order);
      Assert::IsTrue(result == moplexes);

      //everything cached but a new edge between 5 and 6, invalidating 6 as moplex

      result.clear();
      result.push_back(set<int> { 0, 1 });
      result.push_back(set<int> { 2 });
      result.push_back(set<int> { 3 });
      result.push_back(set<int> { 4 });
      result.push_back(set<int> { 7 });

      edges.clear();
      edges.emplace(5, 6);
      graph.add_edge(5, 6);
      moplexes = MinimumFillIn::find_moplexes(graph, cache, edges);
      std_sort(moplexes.begin(), moplexes.end(), moplex_order);
      Assert::IsTrue(result == moplexes);
    }

    TEST_METHOD(FindMinFillBerryBordat) {
      AdjacencyList graph = SampleGraphs::berry_bordat();
      stack<pair<int, int>> edges = MinimumFillIn::minimum_fill_in(graph);
      Assert::AreEqual(4, (int)edges.size());
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

      //6 nodes in a diamond shape. Solvable with one.
      graph = AdjacencyList(6);
      graph.add_edge(0, 1);
      graph.add_edge(1, 4);
      graph.add_edge(0, 2);
      graph.add_edge(0, 3);
      graph.add_edge(3, 4);

      result = MinimumFillIn::minimum_fill_in(graph);
      Assert::IsTrue(1 == result.size());

      //10 nodes in a diamond shape. Solvable with one
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

  TEST_CLASS(MinFillPerformance) {
  public:
    TEST_METHOD(VeryBigCycle) {
      AdjacencyList graph = BuildNCycle(600);
      GraphIO::GraphContext context(graph);
      auto edges = MinimumFillIn::minimum_fill_in(graph);
      Assert::AreEqual(597, (int)edges.size());
    }

    TEST_METHOD(VeryBigFullyConnected) {
      AdjacencyList graph = BuildNFully(1500);
      GraphIO::GraphContext context(graph);
      auto edges = MinimumFillIn::minimum_fill_in(graph);
      Assert::AreEqual(0, (int)edges.size());
    }

    TEST_METHOD(VeryBigSub) {
      AdjacencyList graph = BuildNFullyHalfDeleted(1500);
      GraphIO::GraphContext context(graph);
      auto edges = MinimumFillIn::minimum_fill_in(graph);
      while (!edges.empty()) {
        pair<int, int> edge = edges.top();
        edges.pop();
        graph.add_edge(edge.first, edge.second);
      }
      LexBFS lex(graph.num_vertices);
      lex.order(graph);
      Assert::IsTrue(lex.is_chordal(graph));
    }

    TEST_METHOD(VeryBigHalf) {
      AdjacencyList graph = BuildNHalfFull(15);
      GraphIO::GraphContext context(graph);
      auto edges = MinimumFillIn::minimum_fill_in(graph);
      while (!edges.empty()) {
        pair<int, int> edge = edges.top();
        edges.pop();
        graph.add_edge(edge.first, edge.second);
      }
      LexBFS lex(graph.num_vertices);
      lex.order(graph);
      Assert::IsTrue(lex.is_chordal(graph));
    }
  };

  TEST_CLASS(MinFillInstances) {
  public:
    TEST_METHOD(Instance5) {
      Assert::AreEqual(21, solve_instance("5.graph"));
    }

    TEST_METHOD(Instance9) {
      Assert::AreEqual(9, solve_instance("9.graph"));
    }

    TEST_METHOD(Instance12) {
      Assert::AreEqual(0, solve_instance("12.graph"));
    }

    TEST_METHOD(Instance20) {
      Assert::AreEqual(2, solve_instance("20.graph"));
    }

    TEST_METHOD(Instance43) {
      Assert::AreEqual(0, solve_instance("43.graph"));
    }

    TEST_METHOD(Instance50) {
      Assert::AreEqual(3, solve_instance("50.graph"));
    }

    TEST_METHOD(Instance50_test) {
      GraphIO::GraphContext context = GraphIO::read_from_path("../../CPPace.Tests/problem-instances/50.graph");
      set<pair<int, int>> result;
      result.emplace(21, 26);
      result.emplace(9, 20);
      context.graph.add_edge(21, 26);
      context.graph.add_edge(9, 20);
      bool chordal = MCS::is_chordal(context.graph);
      vector<int> cycle;
      Kernelizer::find_chordless_cycle(context.graph, cycle);
      int i;
      if(chordal) {
        i = 0 * 0;
        i++;
      }
      else {
        i = 1;
        i++;
      }
    }

    TEST_METHOD(Instance74) {
      Assert::AreEqual(0, solve_instance("74.graph"));
    }

    TEST_METHOD(Instance84) {
      Assert::AreEqual(15, solve_instance("84.graph"));
    }

  private:
    int solve_instance(string name) {
      const int n = 10;
      int max_solution = 0;
      GraphIO::GraphContext context = GraphIO::read_from_path("../../CPPace.Tests/problem-instances/" + name);
      for (int i = 0; i < n; i++) {
        int k = MinimumFillIn::minimum_fill_in(context).size();
        max_solution = max_solution > k ? max_solution : k;
      }
      return max_solution;
    }
  };
}

