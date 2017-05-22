#include "stdafx.h"
#include "CppUnitTest.h"
#include "../CPPace.Lib/LexBFS.h"
#include "../CPPace.Lib/Kernelizer.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace CPPaceTests {
  TEST_CLASS(KernelizerTests) {
private: 
  static bool is_chordal_given_order(AdjacencyList& graph, vector<int> order)
  {
    set<int> visited_vertices;
    AdjacencyList visited_graph(graph.num_vertices);
    for (int i = 0; i < graph.num_vertices; i++) {
      int u = order[i];
      set<int> adjacency = graph.edges(u);
      for (int v : adjacency) {
        if (visited_vertices.find(v) != visited_vertices.end())
          visited_graph.add_edge(u, v);
      }

      if (!graph.is_clique(adjacency))
        return false;


      visited_vertices.emplace(u);
    }
    return true;
  }

public:

    TEST_METHOD(MCSBerryBordatOrder) {
      AdjacencyList graph = SampleGraphs::berry_bordat();
      vector<int> order = Kernelizer::MCS(graph);
      
      Assert::IsFalse(is_chordal_given_order(graph, order));

      LexBFS lex(graph.num_vertices);
      lex.order(graph);
    }
  };
}