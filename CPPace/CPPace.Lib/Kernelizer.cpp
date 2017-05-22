#include "Kernelizer.h"
#include <map>
#include <queue>

Kernel Kernelizer::phase1(AdjacencyList& graph)
{
  Kernel kernel;
  kernel.chordless_cycles = 0;
  for (int v : *graph.adjacency_list) {
    kernel.b.emplace(v);
  }

  bool found_cycle;
  do {
    found_cycle = false;
    vector<int> cycle;
    //graph.subgraph
    found_cycle = find_chordless_cycle(graph, cycle);
    if(found_cycle) {
      kernel.chordless_cycles += cycle.size();
      for (int v : cycle) {
        kernel.a.emplace(v);
        kernel.b.erase(v);
      }
    }
  } while (found_cycle);

  return kernel;
}


bool Kernelizer::find_chordless_cycle(AdjacencyList& graph, vector<int>& cycle)
{
  //missing components and subgraph

  vector<int> order = MCS(graph);

  for (int i = 0; i < order.size(); i++) {

    //find m_adjecent:

    set<int> m_adjacent;
    set<int> neighbours = graph.edges(order[i]);

    auto it = next(order.begin(),i);
    while(it != std::prev(order.end())) { // only way i could get O(v*log(v))
      if (neighbours.find(*it) != neighbours.end())
        m_adjacent.emplace(*it);
      it = next(it);
    }
    
    //find cycle from m_adjecent
    if(!graph.is_clique(m_adjacent)) {
      //put m_adjecent into list
      vector<int> m_adjecent_list;
      m_adjecent_list.reserve(m_adjacent.size());
      for (int n : m_adjacent)
        m_adjecent_list.emplace_back(n);

      for(int j = 0; j < m_adjecent_list.size(); j++) {
        int v = m_adjecent_list[j];
        for(int k = j+1; k < m_adjecent_list.size(); k++) {
          int w = m_adjecent_list[k];

          set<int> subgraph_set(*graph.adjacency_list);
          subgraph_set.erase(order[i]);
          for (int n : m_adjacent) {
            if (n != v && n != w)
              subgraph_set.erase(n);
          }
          // graph.subgraph(subgraph_set);
          if(!graph.has_edge(v, w)) {
            vector<int> path;
            if(BFS_path(graph, v, w, path) && path.size() > 3) {
              for (int n : path) {
                cycle.push_back(n);
              }
              cycle.push_back(order[i]);
              return true;
            }
          }
        }
      }
    }
  }
  return false;
}

vector<int> Kernelizer::MCS(AdjacencyList& graph)
{
  vector<int> order;
  order.reserve(graph.num_vertices);
  set<int> numbered;
  map<int, int> weight;

  for (int v : *graph.adjacency_list) {
    weight.insert(v, 0);
    order.push_back(v);
  }

  for(int i = graph.num_vertices; i >= 0; i--) {
    //maximum unnumbered vertex:
    int max_unnumbered = -1, value = INT_MIN;
    for(map<int, int>::value_type& kv : weight) {
      if(numbered.find(kv.first) == numbered.end() && kv.second > value) {
        max_unnumbered = kv.first, value = kv.second;
      }
    }

    order[i] = max_unnumbered;
    numbered.emplace(max_unnumbered);

    for(int v : graph.edges(max_unnumbered)) {
      if(numbered.find(v) == numbered.end()) {
        weight[v]++;
      }
    }
  }
  return order;
}

bool Kernelizer::BFS_path(AdjacencyList& graph, int start, int end, vector<int>& path)
{
  vector<int> from_vector;
  from_vector.reserve(graph.num_vertices);
  for(int i = 0; i < graph.num_vertices; i++) {
    from_vector[i] = -1;
  }
  from_vector[start] = start;
  queue<int> q;
  q.push(start);
  bool found = false;
  while(!q.empty() && !found) {
    int current = q.front();
    q.pop();
    for (int n : graph.edges(current)) {
      if(from_vector[n] == -1) {
        from_vector[n] = current;
        q.push(n);
      }
      if (n == end) {
        found = true;
        break;
      }
    }
  }

  if (from_vector[end] == -1)
    return false;

  int path_node = end;
  path.emplace_back(end);
  while(path_node != from_vector[path_node]) {
    path_node = from_vector[path_node];
    path.emplace_back(path_node);
  }
  return true;
}


