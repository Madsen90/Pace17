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
      AdjacencyList graph(3);
      graph.add_edge(0, 1);
      graph.add_edge(1, 2);

      LexBFS lex(graph.num_vertices);
      lex.order(graph);

      Assert::IsTrue(lex.is_chordal(graph));
    }
  };

}