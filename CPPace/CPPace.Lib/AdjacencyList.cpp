#include "AdjacencyList.h"
#include <vector>
#include <queue>

#include "ConsList.h"

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

bool AdjacencyList::is_connected(int u, int v) {
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

bool AdjacencyList::is_clique(set<int>& vertices) {
  for (int u : vertices)
    for (int v : vertices)
      if (!has_edge(u, v) && u != v)
        return false;

  return true;
}

vector<pair<int, int>> AdjacencyList::all_edges() {
  vector<pair<int, int>> result;
  result.reserve(num_edges);
  for (int u = 0; u < num_vertices; u++) {
    for (int v : edges(u)) {
      if (v < u) continue;
      result.emplace_back(u, v);
    }
  }
  return result;
}

bool AdjacencyList::is_path_chordless(vector<int>& path) {
  set<int> path_set;
  for (int u : path) path_set.emplace(u);

  for (unsigned int i = 0; i < path.size() - 1; i++) {
    int u = path[i];
    int next = path[i + 1];
    for (int v : edges(u)) {
      if (path_set.find(v) != path_set.end() && v != next)
        return false;
    }
    path_set.erase(u);
  }

  return true;
}

bool AdjacencyList::find_four_cycle(vector<int>& result) {
  for (int t = 0; t < num_vertices; t++) {
    for (int u : edges(t)) {
      for (int v : edges(u)) {
        if (t == v) continue;
        for (int w : edges(v)) {
          if (u == w) continue;
          for (int x : edges(w)) {
            if (x != t || has_edge(t, v) || has_edge(u, w)) continue;
            result = vector<int> { t, u, v, w };
            return true;
          }
        }
      }
    }
  }
  result = vector<int>();
  return false;
}

bool AdjacencyList::find_v_star(int x, int y, set<int>& moplex, int& v_star) {
  queue<ConsList<int>> q;
  vector<vector<int>> paths;
  ConsList<int> next = ConsList<int>(x);

  do {
    for (int n : edges(next.value)) {
      if (n == y) {
        vector<int> path = next.cons(n).to_vector();
        if (is_path_chordless(path))
          paths.emplace_back(path);
      }
      else if (moplex.find(n) == moplex.end())
        q.push(next.cons(n));
    }
  } while (!q.empty());

  if (paths.empty())
    return false;

  v_star = paths[0][1];
  if (paths.size() == 1)
    return true;

  //check x-v*
  bool found_v_star = true;
  for (int i = 1; i < paths.size(); i++) {
    if (paths[i][1] != v_star) {
      found_v_star = false;
      break;
    }
  }
  if (found_v_star)
    return true;

  //check y-v*
  found_v_star = true;
  v_star = paths[0][paths[0].size() - 1];
  for (int i = 1; i < paths.size(); i++) {
    vector<int> path = paths[i];
    if (path[path.size() - 1] != v_star) {
      found_v_star = false;
      break;
    }
  }
  return found_v_star;
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

void AdjacencyList::make_clique(set<int>& vertices) {
  for (int u : vertices)
    for (int v : vertices)
      if (u != v)
        add_edge(u, v);
}
