#include "GraphGenerator.h"

AdjacencyList GraphGenerator::random(int num_vertices, int num_edges) {
  AdjacencyList graph(num_vertices);
  for (int i = 0; i < num_edges; i++) {
    int u, v;
    do {
      u = rand() % num_vertices;
      v = rand() % num_vertices;
    } while (u == v || graph.has_edge(u, v));
    graph.add_edge(u, v);
  }

  return graph;
}
