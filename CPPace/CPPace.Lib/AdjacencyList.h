#pragma once
#include "EXPORT.h"

#include <set>
#include <vector>
#include "DisjointSet.h"

using namespace std;

class DLLEXPORT AdjacencyList {
public:
  int num_edges;
  int num_vertices;
  set<int>* adjacency_list;
  
  // Useful array for marking vertices.
  // Zero-out before use - leave dirty after use.
  bool* visited;

  AdjacencyList(int num_vertices);

  set<int> edges(int v);
  bool has_edge(int u, int v);
  bool is_connected(int u, int v);
  bool is_clique(set<int>& vertices);
  vector<pair<int, int>> all_edges();
  
  void add_edge(int u, int v);
  void remove_edge(int u, int v);
  void make_clique(set<int>& vertices);

private:
  // Vertex labels differentiating components of the graph.
  // Labels are lazily updated when needed and connectivity_dirty is set.
  int* connectivity_labels;
  bool connectivity_dirty;

  void regenerate_connectivity();
};
