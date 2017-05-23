#include "stdafx.h"
#include "CppUnitTest.h"
#include "../CPPace.Lib/LexBFS.h"
#include <iostream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace CPPaceTests {
  TEST_CLASS(LexBFSTests) {
  public:

    TEST_METHOD(Constructor) {
      const int n = 5;
      LexBFS lex(n);
      Assert::AreEqual(n, lex.num_vertices);
    }

    TEST_METHOD(BerryBordatOrder) {
      AdjacencyList graph = SampleGraphs::berry_bordat();

      LexBFS lex(graph.num_vertices);
      lex.order(graph);

      Assert::IsFalse(lex.is_chordal(graph));
    }

    TEST_METHOD(IsChordal) {
      //Single node
      AdjacencyList graph(4);
      graph.add_vertex(0);

      LexBFS lex(graph.num_vertices);
      lex.order(graph);
      Assert::IsTrue(lex.is_chordal(graph));

      //Two single nodes
      graph.add_vertex(1);

      lex = LexBFS(graph.num_vertices);
      lex.order(graph);
      Assert::IsTrue(lex.is_chordal(graph));

      //Line of two nodes
      graph.add_edge(0, 1);

      lex = LexBFS(graph.num_vertices);
      lex.order(graph);
      Assert::IsTrue(lex.is_chordal(graph));

      //Line of three node
      graph.add_edge(1, 2);

      lex = LexBFS(graph.num_vertices);
      lex.order(graph);
      Assert::IsTrue(lex.is_chordal(graph));

      //4-Cycle
      graph.add_edge(2, 3);
      graph.add_edge(0, 3);

      lex = LexBFS(graph.num_vertices);
      lex.order(graph);
      Assert::IsFalse(lex.is_chordal(graph));

      //Solved 4-cycle
      graph.add_edge(0, 2);

      lex = LexBFS(graph.num_vertices);
      lex.order(graph);
      Assert::IsTrue(lex.is_chordal(graph));

      //Rev 4-cycle
      graph = AdjacencyList(4);
      graph.add_edge(0, 1);
      graph.add_edge(2, 1);
      graph.add_edge(2, 3);
      graph.add_edge(0, 3);
      graph.add_edge(1, 3);

      lex = LexBFS(graph.num_vertices);
      lex.order(graph);
      Assert::IsTrue(lex.is_chordal(graph));
      
      //   0_1_2
      //  /  |  \
      //  3--4--5
      //  \ _|_ /
      //   6 7 8
      graph = AdjacencyList(9);
      graph.add_edge(0, 1);
      graph.add_edge(0, 3);
      graph.add_edge(1, 2);
      graph.add_edge(1, 4);
      graph.add_edge(2, 5);
      graph.add_edge(3, 4);
      graph.add_edge(3, 6);
      graph.add_edge(4, 5);
      graph.add_edge(4, 7);
      graph.add_edge(5, 8);
      graph.add_edge(6, 7);
      graph.add_edge(7, 8);

      lex = LexBFS(graph.num_vertices);
      lex.order(graph);
      Assert::IsFalse(lex.is_chordal(graph));

      graph.add_edge(0, 4);
      lex = LexBFS(graph.num_vertices);
      lex.order(graph);
      Assert::IsFalse(lex.is_chordal(graph));
      
      graph.add_edge(2, 4);
      lex = LexBFS(graph.num_vertices);
      lex.order(graph);
      Assert::IsFalse(lex.is_chordal(graph));
      
      graph.add_edge(3, 7);
      lex = LexBFS(graph.num_vertices);
      lex.order(graph);
      Assert::IsFalse(lex.is_chordal(graph));
      
      graph.add_edge(8, 4);
      lex = LexBFS(graph.num_vertices);
      lex.order(graph);
      Assert::IsFalse(lex.is_chordal(graph));
      
      graph.add_edge(8, 0);
      lex = LexBFS(graph.num_vertices);
      lex.order(graph);
      Assert::IsFalse(lex.is_chordal(graph));
      
      graph.add_edge(2, 6);
      lex = LexBFS(graph.num_vertices);
      lex.order(graph);
      Assert::IsFalse(lex.is_chordal(graph));

      graph.add_edge(5, 3);
      lex = LexBFS(graph.num_vertices);
      lex.order(graph);
      Assert::IsFalse(lex.is_chordal(graph));

      graph.add_edge(1, 8);
      lex = LexBFS(graph.num_vertices);
      lex.order(graph);
      Assert::IsFalse(lex.is_chordal(graph));

      graph.add_edge(2, 8);
      lex = LexBFS(graph.num_vertices);
      lex.order(graph);
      Assert::IsFalse(lex.is_chordal(graph));

      graph.add_edge(2, 3);
      lex = LexBFS(graph.num_vertices);
      lex.order(graph);
      Assert::IsFalse(lex.is_chordal(graph));

      graph.add_edge(1, 3);
      lex = LexBFS(graph.num_vertices);
      lex.order(graph);
      Assert::IsFalse(lex.is_chordal(graph));

      graph.add_edge(7, 2);
      lex = LexBFS(graph.num_vertices);
      lex.order(graph);
      Assert::IsFalse(lex.is_chordal(graph));
      
      graph.add_edge(8, 3);
      lex = LexBFS(graph.num_vertices);
      lex.order(graph);
      Assert::IsTrue(lex.is_chordal(graph));
    }

    TEST_METHOD(SubGraphIsChordal) {
      //4-cycle, exclude 1
      AdjacencyList graph(4);
      graph.add_edge(0, 1);
      graph.add_edge(1, 2);
      graph.add_edge(2, 3);
      graph.add_edge(3, 0);

      LexBFS lex(graph.num_vertices);
      lex.order(graph);
      Assert::IsFalse(lex.is_chordal(graph));

      graph.remove_vertex(0);

      lex = LexBFS(graph.num_vertices);
      lex.order(graph);
      Assert::IsTrue(lex.is_chordal(graph));

      //5-cycle, exclude 2 (Partitioning)
      graph = AdjacencyList(5);
      graph.add_edge(0, 1);
      graph.add_edge(1, 2);
      graph.add_edge(2, 3);
      graph.add_edge(3, 4);
      graph.add_edge(4, 0);

      lex = LexBFS(graph.num_vertices);
      lex.order(graph);
      Assert::IsFalse(lex.is_chordal(graph));

      graph.remove_vertex(2);
      graph.remove_vertex(3);

      lex = LexBFS(graph.num_vertices);
      lex.order(graph);
      Assert::IsTrue(lex.is_chordal(graph));
      
      //6-cycle with noise
      graph = SampleGraphs::multiple_moplexes_and_big_cc();
      lex = LexBFS(graph.num_vertices);
      lex.order(graph);
      Assert::IsFalse(lex.is_chordal(graph));

      //Break cycle
      graph.remove_vertex(8);
      lex = LexBFS(graph.num_vertices);
      lex.order(graph);
      Assert::IsTrue(lex.is_chordal(graph));

      //Break cycle
      graph = SampleGraphs::multiple_moplexes_and_big_cc();
      lex = LexBFS(graph.num_vertices);
      graph.remove_vertex(4);
      lex.order(graph);
      Assert::IsTrue(lex.is_chordal(graph));

      //Create new cycle
      graph.add_edge(2, 5);
      lex = LexBFS(graph.num_vertices);
      lex.order(graph);
      Assert::IsFalse(lex.is_chordal(graph));
    }
  };

}