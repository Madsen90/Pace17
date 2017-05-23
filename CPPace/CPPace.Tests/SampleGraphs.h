#pragma once

#include "../CPPace.Lib/AdjacencyList.h"

class SampleGraphs {
public:
  static AdjacencyList berry_bordat() {
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
    return graph;
  }

  // Graph with 3-vertex moplex {0,1,2}, 6-vertex cycle, and "false" moplex triangle
  static AdjacencyList multiple_moplexes_and_big_cc()
  {
    AdjacencyList graph(11);
    graph.add_edge(0, 1);
    graph.add_edge(0, 2);
    graph.add_edge(0, 3);
    graph.add_edge(1, 2);
    graph.add_edge(1, 3);
    graph.add_edge(2, 3);
    graph.add_edge(3, 4);
    graph.add_edge(3, 10);
    graph.add_edge(4, 5);
    graph.add_edge(4, 6);
    graph.add_edge(5, 6);
    graph.add_edge(5, 8);
    graph.add_edge(6, 7);
    graph.add_edge(8, 9);
    graph.add_edge(9, 10);
    return graph;
  }
};
