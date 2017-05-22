#include <queue>
#include "MinimumFillIn.h"
#include "ConsList.h"

bool MinimumFillIn::is_path_chordless(AdjacencyList& graph, vector<int>& path) {
  set<int> path_set;
  for (int u : path) path_set.emplace(u);

  for (unsigned int i = 0; i < path.size() - 1; i++) {
    int u = path[i];
    int next = path[i + 1];
    for (int v : graph.edges(u)) {
      if (path_set.find(v) != path_set.end() && v != next)
        return false;
    }
    path_set.erase(u);
  }

  return true;
}

bool MinimumFillIn::find_four_cycle(AdjacencyList& graph, vector<int>& result) {
  for (int t = 0; t < graph.num_vertices; t++) {
    for (int u : graph.edges(t)) {
      for (int v : graph.edges(u)) {
        if (t == v) continue;
        for (int w : graph.edges(v)) {
          if (u == w) continue;
          for (int x : graph.edges(w)) {
            if (x != t || graph.has_edge(t, v) || graph.has_edge(u, w)) continue;
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

bool MinimumFillIn::find_v_star(AdjacencyList& graph, int x, int y, set<int>& moplex, int& v_star) {
  queue<shared_ptr<ConsList<int>>> q;
  vector<vector<int>> paths;
  q.push(make_shared<ConsList<int>>(x));
  for (int i = 0; i < graph.num_vertices; i++)
    graph.visited[i] = false;
  graph.visited[x] = true;

  do {
    shared_ptr<ConsList<int>> next = q.front();
    q.pop();
    for (int n : graph.edges(next->value)) {
      if (n == y) {
        vector<int> path = ConsList<int>::to_vector(ConsList<int>::cons(n, next));
        if (is_path_chordless(graph, path))
          paths.emplace_back(path);
      }
      else if (moplex.find(n) == moplex.end() && !graph.visited[n]) {
        graph.visited[n] = true;
        q.push(ConsList<int>::cons(n, next));
      }
    }
  } while (!q.empty());

  if (paths.empty())
    return false;

  v_star = paths[0][1];
  if (paths.size() == 1)
    return true;

  //check x-v*
  bool found_v_star = true;
  for (unsigned int i = 1; i < paths.size(); i++) {
    if (paths[i][1] != v_star) {
      found_v_star = false;
      break;
    }
  }
  if (found_v_star)
    return true;

  //check y-v*
  found_v_star = true;
  v_star = paths[0][paths[0].size() - 2];
  for (unsigned int i = 1; i < paths.size(); i++) {
    vector<int> path = paths[i];
    if (path[path.size() - 2] != v_star) {
      found_v_star = false;
      break;
    }
  }
  return found_v_star;
}
