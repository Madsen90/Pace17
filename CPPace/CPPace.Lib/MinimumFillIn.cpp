#include "MinimumFillIn.h"
#include "ConsList.h"
#include <algorithm>
#include <iterator>
#include <queue>

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
            result = vector<int>{ t, u, v, w };
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

vector<set<int>> MinimumFillIn::find_moplexes(AdjacencyList& graph) {
  vector<set<int>> moplexes;

  // Exclude inactive vertices from search
  // (graph.visited is already used by connected components)
  vector<bool> visited(graph.num_vertices);
  for (int u = 0; u < graph.num_vertices; u++)
    visited[u] = !graph.vertices[u].active;

  for (int u = 0; u < graph.num_vertices; u++) {
    if (visited[u]) continue;
    visited[u] = true;

    set<int> potential_moplex{ u };
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
      moplexes.push_back(potential_moplex);
  }

  return moplexes;
}


struct MinimumFillInResult {
  int k;
  stack<pair<int, int>> edges;

  MinimumFillInResult(int k, stack<pair<int, int>> edges)
    : k(k),
    edges(edges) {}
};

set<int> add_edge_wrapper(AdjacencyList& graph, int x, int y, set<int>& marked, stack<pair<int, int>>& added) {
  set<int> changed_markings;
  auto x_it = marked.find(x);
  if (x_it != marked.end()) {
    changed_markings.emplace(x);
    marked.erase(x_it);
  }
  auto y_it = marked.find(y);
  if (y_it != marked.end()) {
    changed_markings.emplace(y);
    marked.erase(y_it);
  }
  added.emplace(x, y);
  return changed_markings;
}

set<int> get_neighbourhood_of_moplex(AdjacencyList& graph, set<int>& moplex) {
  set<int> adjacent = graph.edges(*moplex.begin());
  //Remove moplex from adjacent
  for (int n : moplex) {
    auto found = adjacent.find(n);
    if (found != adjacent.end())
      adjacent.erase(found);
  }
  return adjacent;
}

MinimumFillInResult minimum_fill_in_inner(AdjacencyList& graph, int k, int r, stack<pair<int, int>>& added, set<int>& marked) {
  if (k < 0 || r < -1) return MinimumFillInResult(k, added);

  //CASE: Four cycles
  vector<int> four_cycle;
  if (MinimumFillIn::find_four_cycle(graph, four_cycle)) {
    //Branch 1
    set<int> changed_markings = add_edge_wrapper(graph, four_cycle[0], four_cycle[2], marked, added);
    MinimumFillInResult res_branch1 = minimum_fill_in_inner(graph, k - 1, r + changed_markings.size(), added, marked);

    //Reset
    graph.remove_edge(four_cycle[0], four_cycle[2]);
    added.pop();    
    for (int mark : changed_markings)
      marked.emplace(mark);

    //Branch 2
    changed_markings = add_edge_wrapper(graph, four_cycle[1], four_cycle[3], marked, added);
    MinimumFillInResult res_branch2 = minimum_fill_in_inner(graph, k - 1, r + changed_markings.size(), added, marked);

    //Reset
    graph.remove_edge(four_cycle[1], four_cycle[3]);
    added.pop();
    for (int mark : changed_markings)
      marked.emplace(mark);

    if (res_branch1.k > res_branch2.k)
      return res_branch1;
    return res_branch2;
  }

  //Find moplexes
  vector<set<int>> moplexes = MinimumFillIn::find_moplexes(graph);

    bool contains_marked, contains_unmarked = false;

    //CASE: Moplex with marked and unmarked edges
    for (set<int> moplex : moplexes) {
      for (int n : moplex) {
        if (marked.find(n) != marked.end())
          contains_marked = true;
        else
          contains_unmarked = true;
      }
      if (contains_marked && contains_unmarked) {
        //Found a moplex fulfilling the criteria
        for (int n : moplex) {
          marked.emplace(n);
          r--;
        }
        return minimum_fill_in_inner(graph, k, r, added, marked);
      }
    }
    //CASE: Simplicial moplexes with only unmarked edges
    vector<int> unmarked_moplex_indices;
    for (int i = 0; i < moplexes.size(); i++) {

      //Check if moplex is unmarked
      bool is_unmarked_moplex = true;
      for (int n : moplexes[i])
        if (marked.find(n) != marked.end())
          is_unmarked_moplex = false;

      if (is_unmarked_moplex) {
        //unmarked moplex found. Note it
        unmarked_moplex_indices.emplace_back(i);
        //Find adjacent of moplex
        set<int> adjacent = get_neighbourhood_of_moplex(graph, moplexes[i]);

        //Check if neighbours are a clique
        if (graph.is_clique(adjacent)) {
          graph.remove_vertices(adjacent);

          return minimum_fill_in_inner(graph, k, r, added, marked);
        }
      }
    }

    //CASE: Moplex with only unmarked and neighbourhood only missing one edge
    for (int i = 0; i < unmarked_moplex_indices.size(); i++) {
      pair<int, int> missing_edge;
      bool missing_more_than_one = false;

      set<int> moplex_neighbourhood = get_neighbourhood_of_moplex(graph, moplexes[unmarked_moplex_indices[i]]);

      for (int n : moplex_neighbourhood){
        for (int m : moplex_neighbourhood) {
          if (n == m) continue;
          if (!graph.has_edge(n, m))
            if (missing_edge.first == 0 && missing_edge.second == 0)
              missing_edge = pair<int, int>(n, m);
            else {
              missing_more_than_one = true;
              break;
            }
        }
        if (missing_more_than_one) break;
      }

      if (!missing_more_than_one && missing_edge.first != 0 && missing_edge.second != 0) {
        int v_star;
        if (MinimumFillIn::find_v_star(graph, missing_edge.first, missing_edge.second, moplexes[unmarked_moplex_indices[i]], v_star)) {
          if (marked.erase(v_star) != 0) //R might not need to be incremented for v_star
            r++;
          r += add_edge_wrapper(graph, missing_edge.first, missing_edge.second, marked, added).size();

          return minimum_fill_in_inner(graph, k-1, r, added, marked);
        }
      }
    }

    //CASE: All moplexes are marked
    if (unmarked_moplex_indices.size() == 0) {
      return MinimumFillInResult(-1, stack<pair<int, int>>());
   }

    //CASE: Unmarked moplex
    set<int> moplex = *moplexes.begin();

    //Branch 1
    for (int n : moplex) {
      r--;
      marked.emplace(n);
    }

    MinimumFillInResult res_branch1 = minimum_fill_in_inner(graph, k, r, added, marked);

    //Reset
    for (int n : moplex) {
      r++;
      marked.erase(n);
    }
    
    int added_edges_counter = 0;
    set<int> changed_markings;

    //Branch 2
    set<int> neighbourhood = get_neighbourhood_of_moplex(graph, moplex);
    for (int n : neighbourhood) {
      for (int m : neighbourhood) {
        if (n != m && !graph.has_edge(n, m)) {
          added_edges_counter++;
          for (int mark : add_edge_wrapper(graph, n, m, marked, added))
            changed_markings.emplace(mark);
        }
      }
    }

    MinimumFillInResult res_branch2 = minimum_fill_in_inner(graph, k - added_edges_counter, r, added, marked);

    //Reset
    while (added_edges_counter--) added.pop();
    for (int n : changed_markings)
      marked.emplace(n);

    if (res_branch1.k > res_branch2.k)
      return res_branch1;
    return res_branch2;
}

stack<pair<int, int>> MinimumFillIn::minimum_fill_in(AdjacencyList& graph) {
  int k = 0;
  while (true) {
    //KERNELIZE
    MinimumFillInResult res = minimum_fill_in_inner(graph, k, k * 2, stack<pair<int, int>>(), set<int>());
    if (res.k != -1) return res.edges;
  }
  return stack<pair<int, int>>();
}