#include "Kernelizer.h"
#include <queue>

Kernel Kernelizer::phase1(AdjacencyList& graph)
{
  Kernel kernel;
  kernel.chordless_cycles = 0;
  for (int v = 0; v < graph.vertices.size(); v++) {
    if(graph.vertices[v].active)
      kernel.b.emplace(v);
  }

  bool found_cycle;
  do {
    vector<int> cycle;
    found_cycle = find_chordless_cycle(graph, cycle);
    if(found_cycle) {
      kernel.chordless_cycles++; //kMin += cycleSet.size() - 3;
      for (int v : cycle) {
        kernel.a.emplace(v);
        kernel.b.erase(v);
      }
      for (int v : cycle)
        graph.remove_vertex(v);
    }
  } while (found_cycle);

  for (int v : kernel.a) 
    graph.add_vertex(v);

  return kernel;
}

Kernel Kernelizer::phase2(AdjacencyList& graph, Kernel& phase1_kernel)
{
  bool cycle_found;
  do {
    cycle_found = false;

  } while (cycle_found);
  return phase1_kernel;
}



bool Kernelizer::find_chordless_cycle(AdjacencyList& graph, vector<int>& cycle)
{
  //missing components

  map<int, int> order = MCS(graph);
  int i = 0;
  for (auto kv : order) {

    //find m_adjecent:

    set<int> m_adjacent;
    set<int> neighbours = graph.edges(kv.first);

    
    for(int n : neighbours) {
      if (order[n] > kv.second)
        m_adjacent.emplace(n);
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

          m_adjacent.erase(v);
          m_adjacent.erase(w);
          m_adjacent.emplace(kv.first);

          graph.remove_vertices(m_adjacent);
          if(!graph.has_edge(v, w)) {
            vector<int> path;
            if(BFS_path(graph, v, w, path) && path.size() >= 3) { //maybe redudant path size check
              for (int n : path) {
                cycle.push_back(n);
              }
              cycle.push_back(kv.first);
              graph.add_vertices(m_adjacent);
              return true;
            }
          }
          graph.add_vertices(m_adjacent);
          m_adjacent.emplace(v);
          m_adjacent.emplace(w);
          m_adjacent.erase(kv.first);
        }
      }
    }
    i++;
  }
  return false;
}

map<int, int> Kernelizer::MCS(AdjacencyList& graph)
{
  map<int, int> order;
  set<int> numbered;
  map<int, int> weight;
  int order_tmp = 0;
  for (int v = 0; v < graph.vertices.size(); v++) {
    if(!graph.vertices[v].active) continue;
    weight[v] = 0;
  }


  for(int i = weight.size()-1; i >= 0; i--) {
    //maximum weighted unnumbered vertex:
    int max_unnumbered = -1, value = INT_MIN;
    for(map<int, int>::value_type& kv : weight) {
      if(numbered.find(kv.first) == numbered.end() && kv.second > value) {
        max_unnumbered = kv.first, value = kv.second;
      }
    }
    order[max_unnumbered] = i;
    numbered.emplace(max_unnumbered);

    for(int v : graph.edges(max_unnumbered)) {
      if (!graph.vertices[v].active) continue;
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
    from_vector.emplace_back(-1);
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

bool Kernelizer::is_chordal_given_order(AdjacencyList& graph, map<int, int> order) {
  for (int v = 0; v < order.size(); v++) {
    int ordering = order[v];
    set<int> higher_order_neighbours;
    for (int n : graph.edges(v)) {
      if (order.find(n) != order.end() && order[n] > ordering) {
        higher_order_neighbours.emplace(n);
      }
    }

    if (!graph.is_clique(higher_order_neighbours)) {
      return false;
    }
  }
  return true;
}
