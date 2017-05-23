#include "Kernelizer.h"
#include <queue>
#include <iterator>
#include "MinimumFillIn.h"


static bool set_size_comparison(set<int> a, set<int> b)
{
  return a.size() < b.size();
}

Kernel Kernelizer::phase1(AdjacencyList& graph) {
  Kernel kernel;
  kernel.kMin = 0;
  for (int v = 0; v < graph.vertices.size(); v++) {
    if(graph.vertices[v].active)
      kernel.b.emplace(v);
  }

  bool found_cycle;
  do {
    vector<int> cycle;
    found_cycle = find_chordless_cycle(graph, cycle);
    if(found_cycle) {
      kernel.kMin += cycle.size() - 3;
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


bool Kernelizer::phase2(AdjacencyList& graph, Kernel& phase1_kernel) {
  bool cycle_found;
  do {
    cycle_found = false;
    for(int a_vertex : phase1_kernel.a) {
      //find neighbouhood in b
      set<int> neighbourhood_in_B = graph.edges(a_vertex);
      for (int in_a : phase1_kernel.a) {
          neighbourhood_in_B.erase(in_a);
      }

      for(int b_vertex : neighbourhood_in_B) {
        //(N(b_vertex) - N(a_vertex) intersect phase2_kernel.b:
        set<int> R;
        set<int> b_minus_a = graph.edges(b_vertex);
        for (int a_neighbour : graph.edges(a_vertex)) {
          b_minus_a.erase(a_neighbour);
        }
        std::set_intersection(
          b_minus_a.begin(), b_minus_a.end(),
          phase1_kernel.b.begin(), phase1_kernel.b.end(),
          inserter(R, R.begin()));
        graph.remove_vertex(b_vertex);

        for(int v : R) {
          vector<int> path;
          if(BFS_path(graph, a_vertex, v, path)) {
            bool is_chordless = true;
            for (int s : path) {
              if (s == v || s == a_vertex) continue;
              if (graph.has_edge(b_vertex, s, true))
                is_chordless = false;
            }
            if (!is_chordless) continue;
            cycle_found = true;
            path.emplace_back(b_vertex);

            //cycle found. calculate kmin accordingly, by calculating length of cycle's subpaths in B
              //start by finding consequtive subpaths
            vector<set<int>> sub_paths;
            set<int> sub_path;
            bool previous_v_in_b = false;
            for (int p : path) {
              if(previous_v_in_b) {
                if (phase1_kernel.b.find(p) != phase1_kernel.b.end())
                  sub_path.emplace(p);
                else {
                  sub_paths.emplace_back(set<int>(sub_path));
                  sub_path.clear();
                  previous_v_in_b = false;
                }
              } 
              else {
                if (phase1_kernel.b.find(p) != phase1_kernel.b.end()) {
                  sub_path.emplace(p);
                  previous_v_in_b = true;
                }
              }
            }
            if (!sub_path.empty()) sub_paths.emplace_back(sub_path);

            sort(sub_paths.begin(), sub_paths.end(), set_size_comparison);

            if(sub_paths.size() == 1) {
              if(sub_paths.front().size() == path.size() -1) 
                phase1_kernel.kMin += sub_paths.front().size() - 1;
              else
                phase1_kernel.kMin += sub_paths.front().size() - 2;
            }
            else {
              int size_sum = 0;
              for (set<int> s : sub_paths)
                size_sum += s.size();
              size_sum = size_sum / 2;
              phase1_kernel.kMin += max(size_sum, (int)sub_paths.front().size());
            }


            // add cycle to kernel
            for (int cycle_vertex : path) {
              phase1_kernel.a.emplace(cycle_vertex);
              phase1_kernel.b.erase(cycle_vertex);
            }
            break;
          }
        }
        graph.add_vertex(b_vertex);
        if (cycle_found)
          break;
      }
      if (cycle_found)
        break;
    }
  } while (cycle_found);
  return !phase1_kernel.a.empty();
}

bool Kernelizer::phase3(AdjacencyList& graph, Kernel& phase2_kernel, Kernel& phase3_kernel, int max_k)
{
  if (max_k < phase2_kernel.kMin)
    return false;

  int k_prime = max_k;
  phase3_kernel.a = set<int>(phase2_kernel.a);
  phase3_kernel.b = set<int>(phase2_kernel.b);
  phase3_kernel.kMin = phase2_kernel.kMin;

  graph.remove_vertices(phase2_kernel.b);
  vector<pair<int, int>> non_edges = find_non_edges(graph);

  for (int i = 0; i < non_edges.size(); i++) {
    pair<int, int> non_edge = non_edges[i];
    int x = non_edge.first, y = non_edge.second;
    graph.add_vertices(phase2_kernel.b);
    set<int> x_neighbourhood = graph.edges(x);
    set<int> y_neighbourhood = graph.edges(y);
    graph.remove_vertices(phase2_kernel.b);
    set<int> x_y_intersect;
    set<int> x_y_b;

    set_intersection(
      x_neighbourhood.begin(), x_neighbourhood.end(),
      y_neighbourhood.begin(), y_neighbourhood.end(),
      inserter(x_y_intersect, x_y_intersect.begin())    
    );
    set_intersection(
      x_y_intersect.begin(), x_y_intersect.end(),
      phase3_kernel.b.begin(), phase3_kernel.b.end(),
      inserter(x_y_b, x_y_b.begin())
    );

    set<int> a_xy;
    for (int n : x_y_b) {

      graph.add_vertices(phase2_kernel.b);
      set<int> n_neighbours = graph.edges(n);
      n_neighbours.erase(x);
      n_neighbours.erase(y);

      graph.remove_vertices(n_neighbours); // chordless ensurement
      graph.remove_vertex(n);

      vector<int> path;
      if (BFS_path(graph, x, y, path))
        a_xy.emplace(n);
      
      graph.add_vertex(n);
      graph.add_vertices(n_neighbours);
      graph.remove_vertices(phase2_kernel.b);
    }

    if(a_xy.size() > 2*max_k) {
      phase3_kernel.essential_edges.emplace(non_edge);
      k_prime--;
      graph.add_edge(non_edge.first, non_edge.second);
      if (k_prime < 0) return false;
    } else {
      for (int v : a_xy) {
        phase3_kernel.a.emplace(v);
        phase3_kernel.b.erase(v);
      }
    }
  }

  //graph clean-up
  for (pair<int, int> edge : phase3_kernel.essential_edges) {
    graph.remove_edge(edge.first, edge.second);
  }
  graph.add_vertices(phase2_kernel.b);
  return true;
}


bool Kernelizer::find_chordless_cycle(AdjacencyList& graph, vector<int>& cycle) {
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

map<int, int> Kernelizer::MCS(AdjacencyList& graph) {
  map<int, int> order;
  set<int> numbered;
  map<int, int> weight;
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

bool Kernelizer::BFS_path(AdjacencyList& graph, int start, int end, vector<int>& path) {
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

vector<pair<int, int>> Kernelizer::find_non_edges(AdjacencyList& graph)
{
  vector<pair<int, int>> non_edges;
  for (int u = 0; u < graph.num_vertices; u++) {
    if(!graph.vertices[u].active) continue;
    for(int v = u+1; v < graph.num_vertices; v++) {
      if(!graph.vertices[v].active) continue;
      if(graph.has_edge(v, u)) continue;
      non_edges.emplace_back(pair<int, int>(u, v));
    }
  }
  return non_edges;
}

 
