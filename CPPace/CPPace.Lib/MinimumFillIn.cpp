#include "MinimumFillIn.h"
#include "SetFunctions.h"
#include "Log.h"
#include "ConsList.h"
#include "LexBFS.h"
#include "Kernelizer.h"
#include <algorithm>
#include <iterator>
#include <queue>
#include "MCS.h"

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

static bool moplex_has_edge_in_neighbourhood(AdjacencyList& graph, set<int>& moplex, pair<int, int>& edge)
{
  for (int n : get_neighbourhood_of_moplex(graph, moplex)) {
    if (edge.first == n) return true;
    if (edge.second == n) return true;
  }
}

vector<set<int>> MinimumFillIn::find_moplexes(AdjacencyList& graph, vector<set<int>>& previous_moplexes, set<pair<int, int>>& newly_added_edges) {
  vector<set<int>> moplexes;

  // Exclude inactive vertices from search
  // (graph.visited is already used by connected components)
  vector<bool> visited(graph.num_vertices);
  for (int u = 0; u < graph.num_vertices; u++)
    visited[u] = !graph.vertices[u].active;

  // No need to recompute uneffected moplexes
  for (set<int> prev_moplex : previous_moplexes) {
    bool still_valid = true;

    // is_active check
    for (int v : prev_moplex) {
      if (!(still_valid = !visited[v])) 
        break;
    }
    if (!still_valid) continue;

    // if moplex have been affected by a newly added edge, it must be recomputed, i.e. is not valid
    for (pair<int, int> new_edge : newly_added_edges) {
      still_valid = moplex_has_edge_in_neighbourhood(graph, prev_moplex, new_edge);
      still_valid = still_valid && prev_moplex.find(new_edge.first) == prev_moplex.end();
      still_valid = still_valid && prev_moplex.find(new_edge.second) == prev_moplex.end();
      if (!still_valid) break;
    }
    
    // the moplex does not need to be recomputed
    if(still_valid) {
      moplexes.push_back(prev_moplex);
      for (int v : prev_moplex) {
        visited[v] = true;
      }
    }
  }

  // time to find new moplexes
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
        connected_to = SetFunctions::set_intersect_two(components, connected_to);
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
  graph.add_edge(x, y);
  added.emplace(x, y);
  return changed_markings;
}


MinimumFillInResult minimum_fill_in_inner(AdjacencyList& graph, int k, int r, stack<pair<int, int>>& added, set<int>& marked, vector<set<int>>& cached_moplexes, set<pair<int, int>>& parents_new_edges) {
  if (k < 0 || r < -1) return MinimumFillInResult(-1, stack<pair<int, int>>());

  if (k == 0) { // due to kernel and incrementing k, we know that there is only a potential solution when k == 0
    LexBFS lex(graph.num_vertices);
    lex.order(graph);
    if(lex.is_chordal(graph))
      return MinimumFillInResult(k, added);
  }

  //CASE: Four cycles
  vector<int> four_cycle;
  if (MinimumFillIn::find_four_cycle(graph, four_cycle)) {
    //Branch 1
    set<int> changed_markings = add_edge_wrapper(graph, four_cycle[0], four_cycle[2], marked, added);
    parents_new_edges.emplace(four_cycle[0], four_cycle[2]);
    MinimumFillInResult res_branch1 = minimum_fill_in_inner(graph, k - 1, r + changed_markings.size(), added, marked, cached_moplexes, parents_new_edges);
    
    //Reset
    graph.remove_edge(four_cycle[0], four_cycle[2]);
    added.pop();    
    parents_new_edges.erase(pair<int, int>(four_cycle[0], four_cycle[2])); // this should never fail, as new edges are never removed. Instead a new set is made, with the exception of four cycles, where new edges are simply overridden.
    marked = SetFunctions::set_union_two(marked, changed_markings);

    //Branch 2
    changed_markings = add_edge_wrapper(graph, four_cycle[1], four_cycle[3], marked, added);
    parents_new_edges.emplace(four_cycle[1], four_cycle[3]);
    MinimumFillInResult res_branch2 = minimum_fill_in_inner(graph, k - 1, r + changed_markings.size(), added, marked, cached_moplexes, parents_new_edges);

    //Reset
    graph.remove_edge(four_cycle[1], four_cycle[3]);
    added.pop();
    parents_new_edges.erase(pair<int, int>(four_cycle[1], four_cycle[3]));
    marked = SetFunctions::set_union_two(marked, changed_markings);

    if (res_branch1.k > res_branch2.k)
      return res_branch1;
    return res_branch2;
  }

  //Find moplexes
  vector<set<int>> moplexes = MinimumFillIn::find_moplexes(graph, cached_moplexes, parents_new_edges);
  set<pair<int, int>> new_edges; // do not use parents_new_edges or cached_moplexes from this point onwards, as parent branches might still need it.
  bool contains_marked = false, contains_unmarked = false;

  //CASE: Moplex with marked and unmarked members
  for (set<int> moplex : moplexes) {
    for (int n : moplex) {
      if (marked.find(n) != marked.end())
        contains_marked = true;
      else
        contains_unmarked = true;
    }
    if (contains_marked && contains_unmarked) {
      //Found a moplex fulfilling the criteria
      marked = SetFunctions::set_union_two(marked, moplex);
      r -= moplex.size();

      MinimumFillInResult result = minimum_fill_in_inner(graph, k, r, added, marked, moplexes, new_edges);
      for (int n : moplex) {
        marked.erase(n);
        r++;
      }
      return result;
    }
  }
  //CASE: Simplicial moplexes with only unmarked edges
  vector<int> unmarked_moplex_indices;
  for (int i = 0; i < moplexes.size(); i++) {

    //Check if moplex is unmarked
    bool is_unmarked_moplex = true;
    for (int n : moplexes[i]) {
      if (marked.find(n) != marked.end())
        is_unmarked_moplex = false;
    }

    if (is_unmarked_moplex) {
      //unmarked moplex found. Note it
      unmarked_moplex_indices.emplace_back(i);
      //Find adjacent of moplex
      set<int> adjacent = get_neighbourhood_of_moplex(graph, moplexes[i]);
      //Check if neighbours are a clique
      if (graph.is_clique(adjacent)) {
        graph.remove_vertices(moplexes[i]);

        MinimumFillInResult result = minimum_fill_in_inner(graph, k, r, added, marked, moplexes, new_edges);
        graph.add_vertices(moplexes[i]);
        return result;
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
        if (n >= m) continue;
        if (!graph.has_edge(n, m))
          if (missing_edge.first == missing_edge.second)
            missing_edge = pair<int, int>(n, m);
          else {
            missing_more_than_one = true;
            break;
          }
      }
      if (missing_more_than_one) break;
    }

    if (!missing_more_than_one && missing_edge.first != missing_edge.second) { 
      int v_star;
      if (MinimumFillIn::find_v_star(graph, missing_edge.first, missing_edge.second, moplexes[unmarked_moplex_indices[i]], v_star)) {
        if (marked.erase(v_star) != 0) //R might not need to be incremented for v_star
          r++;
        r += add_edge_wrapper(graph, missing_edge.first, missing_edge.second, marked, added).size();
        new_edges.insert(missing_edge); 
        MinimumFillInResult result = minimum_fill_in_inner(graph, k-1, r, added, marked, moplexes, new_edges);
        graph.remove_edge(missing_edge.first, missing_edge.second);
        added.pop(); 
        new_edges.erase(missing_edge);
        return result;
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
  marked = SetFunctions::set_union_two(marked, moplex);
  r -= moplex.size();


  MinimumFillInResult res_branch1 = minimum_fill_in_inner(graph, k, r, added, marked, moplexes, new_edges);

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
        new_edges.emplace(n, m); 
        for (int mark : add_edge_wrapper(graph, n, m, marked, added))
          changed_markings.emplace(mark);
      }
    }
  }

  MinimumFillInResult res_branch2 = minimum_fill_in_inner(graph, k - added_edges_counter, r, added, marked, moplexes, new_edges);

  //Reset
  while (added_edges_counter--) {
    pair<int, int> edge = added.top();
    graph.remove_edge(edge.first, edge.second);
    new_edges.erase(edge);
    added.pop();
  }
  marked = SetFunctions::set_union_two(marked, changed_markings);

  if (res_branch1.k > res_branch2.k)
    return res_branch1;
  return res_branch2;
}

stack<pair<int, int>> MinimumFillIn::minimum_fill_in(GraphIO::GraphContext context) {
  Log::info("Initial kernelization...");
  AdjacencyList graph = context.graph;
  Kernel kernel = Kernelizer::phase1(graph);
  Log::info("Kernelization phase 1 completed (lower bound k = %d)", kernel.kMin);

  if (!Kernelizer::phase2(graph, kernel)) {
    Log::info("Kernelization phase 2 completed (graph is chordal)");
    return stack<pair<int, int>>();
  }
  int k = kernel.kMin;
  Log::info("Kernelization phase 2 completed (lower bound k = %d)", kernel.kMin);
  Log::info("========================================");
  
  while (true) {
    Log::info("Searching for solution (k = %d)", k);
    Kernel k_kernel;
    if (!Kernelizer::phase3(graph, kernel, k_kernel, k)) {
      Log::info("Kernelization phase 3 completed (no solution for k = %d)", kernel.kMin);
      continue;
    }

    int num_edges_removed = 0;
    for (int u : k_kernel.b)
      for (int v : context.graph.edges(u))
        if (u > v) num_edges_removed++;

    Log::info("Kernelization phase 3 completed");
    Log::info("Removed %d vertices (%.2f%%)",
      k_kernel.b.size(),
      (float)k_kernel.b.size() / context.graph.num_vertices * 100);
    Log::info("Removed %d edges (%.2f%%)",
      num_edges_removed,
      (float)num_edges_removed / context.graph.num_edges * 100);

    stack<pair<int, int>> added;
    graph.remove_vertices(k_kernel.b);
    for (pair<int, int> edge : k_kernel.essential_edges) {
      graph.add_edge(edge.first, edge.second);
      added.push(edge);
      k--;
    }
    Log::info("Components in kernel: %d", context.graph.regenerate_connectivity());

    set<int> marked;
    vector<set<int>> empty_moplex_set;
    set<pair<int, int>> empty_edge_set;
    MinimumFillInResult res = minimum_fill_in_inner(graph, k, k * 2, added, marked, empty_moplex_set, empty_edge_set);
    if (res.k != -1) {
      Log::info("Solution found for k = %d", k);
      return res.edges;
    }
    Log::info("No solution found for k = %d", k);

    graph.add_vertices(k_kernel.b);
    for (pair<int, int> edge : k_kernel.essential_edges) {
      graph.remove_edge(edge.first, edge.second);
      k++;
    }
    k++;
  }
}