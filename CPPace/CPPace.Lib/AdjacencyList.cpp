#include "AdjacencyList.h"
#include <vector>

AdjacencyList::AdjacencyList(int num_vertices)
  : num_edges(0),
    num_vertices(num_vertices),
    adjacency_list(new set<int>[num_vertices]),
    visited(new bool[num_vertices]),
    connectivity_labels(new int[num_vertices]),
    connectivity_dirty(false)
{ }

set<int> AdjacencyList::edges(int v) {
  return adjacency_list[v];
}

bool AdjacencyList::has_edge(int u, int v) {
  return adjacency_list[u].find(v) != adjacency_list[u].end();
}

void AdjacencyList::add_edge(int u, int v) {
  if (has_edge(u, v)) return;
  adjacency_list[u].emplace(v);
  adjacency_list[v].emplace(u);
  num_edges++;
  connectivity_dirty = true;
}

void AdjacencyList::remove_edge(int u, int v) {
  if (!has_edge(u, v)) return;
  adjacency_list[u].erase(v);
  adjacency_list[v].erase(u);
  num_edges--;
  connectivity_dirty = true;
}

bool AdjacencyList::connected(int u, int v) {
  // Regenerate connectivity labels only when needed
  if (connectivity_dirty)
    regenerate_connectivity();

  return connectivity_labels[u] == connectivity_labels[v];
}

void AdjacencyList::regenerate_connectivity() {
  for (int i = 0; i < num_vertices; i++)
    visited[i] = false;

  int label = 0;
  vector<int> stack;
  for (int i = 0; i < num_vertices; i++)
  {
    if (visited[i]) continue;
    stack.emplace_back(i);

    while (!stack.empty()) {
      int u = stack.back();
      stack.pop_back();

      connectivity_labels[u] = label;
      visited[u] = true;

      for (int v : edges(u)) {
        if (visited[v]) continue;
        stack.emplace_back(v);
      }
    }

    label++;
  }

  connectivity_dirty = false;
}
