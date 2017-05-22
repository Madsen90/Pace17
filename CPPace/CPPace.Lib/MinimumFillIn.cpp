#include <queue>
#include "MinimumFillIn.h"
#include "ConsList.h"
#include <algorithm>
#include <iterator>

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

set<set<int>> MinimumFillIn::find_moplexes(AdjacencyList& graph) {
  set<set<int>> moplexes;

  // Exclude inactive vertices from search
  // (graph.visited is already used by connected components)
  vector<bool> visited(graph.num_vertices);
  for (int u = 0; u < graph.num_vertices; u++)
    visited[u] = !graph.vertices[u].active;

  for (int u = 0; u < graph.num_vertices; u++) {
    if (visited[u]) continue;
    visited[u] = true;

    set<int> potential_moplex { u };
    set<int> u_edges = graph.edges(u); // Modified during loop
    for (int v : graph.edges(u)) {
      set<int> v_edges = graph.edges(v);
      v_edges.erase(u);
      u_edges.erase(v);

      if (v_edges == u_edges) {
        visited[v] = true;
        potential_moplex.emplace(v);
      }

      // Undo change for next iteration
      u_edges.emplace(v);
    }

    // Remove moplex and seperator
    set<int> seperator;
    set_difference(
      u_edges.begin(), u_edges.end(),
      potential_moplex.begin(), potential_moplex.end(),
      inserter(seperator, seperator.begin()));

    graph.remove_vertices(seperator);
    graph.remove_vertices(potential_moplex);

    // Check if each vertex of seperator is connected to all components
    int num_components = graph.regenerate_connectivity();
    set<int> connected_to;
    bool is_first = true;
    bool is_moplex = true;
    for (int v : seperator) {
      graph.add_vertex(v);
      
      // Set of components v is connected to
      set<int> components;
      for (int w : graph.edges(v))
        components.emplace(graph.vertices[w].label);

      if (is_first) {
        is_first = false;
        connected_to = components;
      }
      else {
        set<int> component_intersection;
        set_intersection(
          components.begin(), components.end(),
          connected_to.begin(), connected_to.end(),
          inserter(component_intersection, component_intersection.begin()));
        connected_to = component_intersection;
      }
      graph.remove_vertex(v);

      if (connected_to.empty()) {
        is_moplex = false;
        break;
      }
    }
    
    graph.add_vertices(seperator);
    graph.add_vertices(potential_moplex);

    if (is_moplex)
      moplexes.insert(potential_moplex);
  }

  return moplexes;
}
