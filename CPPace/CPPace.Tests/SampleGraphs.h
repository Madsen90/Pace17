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
};
