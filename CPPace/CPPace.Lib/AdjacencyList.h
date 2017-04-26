#pragma once

#ifdef ADJACENCYLIST_EXPORTS
#define ADJACENCYLIST_API _declspec(dllexport)
#else
#define ADJACENCYLIST_API _declspec(dllimport)
#endif

#include <set>
#include "DisjointSet.h"

using namespace std;

class ADJACENCYLIST_API AdjacencyList {
public:
  int num_edges;
  int num_vertices;
  set<int>* adjacency_list;

  // Useful array for marking vertices.
  // Zero-out before use - leave dirty after use.
  bool* visited;

  // Vertex labels differentiating components of the graph.
  // Labels are lazily updated when needed and connectivity_dirty is set.
  int* connectivity_labels;
  bool connectivity_dirty;

  AdjacencyList(int num_vertices);

  set<int> edges(int v);
  bool has_edge(int u, int v);
  void add_edge(int u, int v);
  void remove_edge(int u, int v);
  bool connected(int u, int v);
  void regenerate_connectivity();
};
