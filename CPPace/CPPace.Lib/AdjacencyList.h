#pragma once
#include "EXPORT.h"

#include <set>
#include <vector>

using namespace std;

struct Vertex {
  bool active;
  set<int> adjacency;

  Vertex()
    : active(true),
    adjacency() {}
};

class DLLEXPORT AdjacencyList {
public:
  int num_edges;
  int num_vertices;
  vector<Vertex> vertices;

  // Useful vector for marking vertices.
  // Zero-out before use - leave dirty after use.
  vector<bool> visited;

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

private:
  // Vertex labels differentiating components of the graph.
  // Labels are lazily updated when needed and connectivity_dirty is set.
  int* connectivity_labels;
  bool connectivity_dirty;

  void regenerate_connectivity();
};
