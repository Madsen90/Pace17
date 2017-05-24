#pragma once
#include "EXPORT.h"

#include <set>
#include <vector>

using namespace std;

struct Vertex {
  set<int> adjacency;
  set<int> active_adjacency_cache;
  bool active;
  bool active_adjacency_cache_dirty;
  
  // Label differentiating components of the graph.
  int label;

  Vertex()
    : adjacency(),
      active_adjacency_cache(),
      active(true),
      active_adjacency_cache_dirty(true)
  {}
};

class DLLEXPORT AdjacencyList {
public:
  int num_edges;
  int num_vertices;
  vector<Vertex> vertices;

  // Useful vector for marking vertices.
  // Zero-out before use - leave dirty after use.
  vector<bool> visited;

  // Component labels are lazily updated when needed and connectivity_dirty is set.
  bool connectivity_dirty;

  AdjacencyList(int num_vertices);

  set<int> edges(int v);
  bool has_edge(int u, int v, bool include_inactive = false);
  bool is_connected(int u, int v);
  bool is_clique(set<int>& vertices);
  vector<pair<int, int>> all_edges();
  
  void add_edge(int u, int v);
  void remove_edge(int u, int v);
  void make_clique(set<int>& vertices);
  void add_vertex(int u);
  void add_vertices(set<int> vertices);
  void add_all_vertices();
  void remove_vertex(int u);
  void remove_vertices(set<int> vertices);
  void set_vertices(set<int> vertices);
  int regenerate_connectivity();
};
