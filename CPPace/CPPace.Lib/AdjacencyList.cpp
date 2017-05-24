#include "AdjacencyList.h"
#include <vector>
#include <queue>

AdjacencyList::AdjacencyList(int num_vertices)
  : num_edges(0),
    num_vertices(num_vertices),
    vertices(),
    visited(),
    connectivity_dirty(false) {
  vertices.resize(num_vertices);
  visited.resize(num_vertices);
}

set<int> AdjacencyList::edges(int u) {
  if (!vertices[u].active_adjacency_cache_dirty)
    return vertices[u].active_adjacency_cache;

  set<int> result;
  if (!vertices[u].active) return result;
  for (int v : vertices[u].adjacency)
    if (vertices[v].active)
      result.emplace(v);

  vertices[u].active_adjacency_cache = result;
  vertices[u].active_adjacency_cache_dirty = false;

  return result;
}

// TODO: Use edges()?
bool AdjacencyList::has_edge(int u, int v, bool include_inactive) {
  return (include_inactive || (vertices[u].active && vertices[v].active)) &&
    vertices[u].adjacency.find(v) != vertices[u].adjacency.end();
}

bool AdjacencyList::is_connected(int u, int v) {
  if (!vertices[u].active || !vertices[v].active)
    return false;

  // Regenerate connectivity labels only when needed
  if (connectivity_dirty)
    regenerate_connectivity();

  return vertices[u].label == vertices[v].label;
}

int AdjacencyList::regenerate_connectivity() {
  // Exclude inactive vertices from search
  for (int i = 0; i < num_vertices; i++)
    visited[i] = !vertices[i].active;

  int label = 0;
  vector<int> stack;
  for (int i = 0; i < num_vertices; i++)
  {
    if (visited[i]) continue;
    stack.emplace_back(i);

    while (!stack.empty()) {
      int u = stack.back();
      stack.pop_back();

      vertices[u].label = label;
      visited[u] = true;

      for (int v : edges(u)) {
        if (visited[v]) continue;
        stack.emplace_back(v);
      }
    }
    label++;
  }
  connectivity_dirty = false;
  return label;
}

bool AdjacencyList::is_clique(set<int>& vertice_set) {
  for (int u : vertice_set) {
    if (!vertices[u].active)
      return false;

    for (int v : vertice_set)
      if (!has_edge(u, v) && u != v)
        return false;
  }
  return true;
}

vector<pair<int, int>> AdjacencyList::all_edges() {
  vector<pair<int, int>> result;
  result.reserve(num_edges);
  for (int u = 0; u < num_vertices; u++) {
    if (!vertices[u].active) continue;
    for (int v : edges(u)) {
      if (v < u || !vertices[v].active) continue;
      result.emplace_back(u, v);
    }
  }
  return result;
}

void AdjacencyList::add_edge(int u, int v) {
  if (has_edge(u, v, true)) return;
  vertices[u].adjacency.emplace(v);
  vertices[v].adjacency.emplace(u);
  num_edges++;
  connectivity_dirty = true;
  vertices[u].active_adjacency_cache_dirty = true;
  vertices[v].active_adjacency_cache_dirty = true;
}

void AdjacencyList::remove_edge(int u, int v) {
  if (!has_edge(u, v, true)) return;
  vertices[u].adjacency.erase(v);
  vertices[v].adjacency.erase(u);
  num_edges--;
  connectivity_dirty = true;
  vertices[u].active_adjacency_cache_dirty = true;
  vertices[v].active_adjacency_cache_dirty = true;
}

void AdjacencyList::make_clique(set<int>& vertices) {
  for (int u : vertices)
    for (int v : vertices)
      if (u != v)
        add_edge(u, v);
}

void AdjacencyList::add_vertex(int u) {
  if (vertices[u].active) return;

  vertices[u].active = true;
  connectivity_dirty = true;
  
  vertices[u].active_adjacency_cache_dirty = true;
  for (int v : edges(u))
    vertices[v].active_adjacency_cache_dirty = true;
}

void AdjacencyList::add_vertices(set<int> vertices) {
  for (int u : vertices)
    add_vertex(u);
}

void AdjacencyList::add_all_vertices() {
  for (int u = 0; u < num_vertices; u++)
    add_vertex(u);
}

void AdjacencyList::remove_vertex(int u) {
  if (!vertices[u].active) return;

  for (int v : edges(u))
    vertices[v].active_adjacency_cache_dirty = true;

  vertices[u].active = false;
  connectivity_dirty = true;

  vertices[u].active_adjacency_cache_dirty = true;
}

void AdjacencyList::remove_vertices(set<int> vertices) {
  for (int u : vertices)
    remove_vertex(u);
}

void AdjacencyList::set_vertices(set<int> vertices) {
  int u = 0;
  for (int v : vertices) {
    while (u < v)
      remove_vertex(u++);
    add_vertex(v);
    u++;
  }
  for (; u < num_vertices; u++)
    remove_vertex(u);
}
