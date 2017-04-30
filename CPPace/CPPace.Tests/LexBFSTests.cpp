#include "stdafx.h"
#include "CppUnitTest.h"
#include "../CPPace.Lib/LexBFS.h"

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

      LexBFS lex(graph.num_vertices);
      lex.order(graph);

      Assert::IsFalse(lex.is_chordal(graph));
    }
  };
}