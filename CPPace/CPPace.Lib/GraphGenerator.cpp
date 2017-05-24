#include "GraphGenerator.h"
#include <cstdlib>

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

AdjacencyList GraphGenerator::fully_connected(int n) {
  //Fully connected with N nodes
  AdjacencyList graph(n);
  for (int i = 0; i < n; i++) {
    for (int j = 1; j < n; j++) {
      graph.add_edge(i, j);
    }
  }
  return graph;
}

AdjacencyList GraphGenerator::half_connected(int n) {
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

AdjacencyList GraphGenerator::cycle(int n) {
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
