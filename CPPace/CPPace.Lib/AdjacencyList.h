#pragma once

#ifdef ADJACENCYLIST_EXPORTS
#define ADJACENCYLIST_API _declspec(dllexport)
#else
#define ADJACENCYLIST_API _declspec(dllimport)
#endif

#include <set>
#include "DisjointSet.h"
#include <vector>

using namespace std;

class ADJACENCYLIST_API AdjacencyList {
public:
  int num_edges;
  int num_vertices;
  set<int>* adjacency_list;

  AdjacencyList(int num_vertices);

  set<int> edges(int v);
  bool has_edge(int u, int v);
  bool is_connected(int u, int v);
  bool is_clique(set<int>& vertices);
  vector<pair<int, int>> all_edges();
  bool is_path_chordless(vector<int>& path);

  void add_edge(int u, int v);
  void remove_edge(int u, int v);
  void make_clique(set<int>& vertices);

private:
  // Useful array for marking vertices.
  // Zero-out before use - leave dirty after use.
  bool* visited;

  // Vertex labels differentiating components of the graph.
  // Labels are lazily updated when needed and connectivity_dirty is set.
  int* connectivity_labels;
  bool connectivity_dirty;

  void regenerate_connectivity();
};
