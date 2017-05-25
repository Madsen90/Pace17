#pragma once

#include <map>
#include <climits>
#include "AdjacencyList.h"

using namespace std;

class DLLEXPORT MCS {
public:

  static map<int, int> order(AdjacencyList& graph) {
    map<int, int> order;
    set<int> numbered;
    map<int, int> weight;
    for (int v = 0; v < graph.vertices.size(); v++) {
      if (!graph.vertices[v].active) continue;
      weight[v] = 0;
    }


    for (int i = weight.size() - 1; i >= 0; i--) {
      //maximum weighted unnumbered vertex:
      int max_unnumbered = -1, value = INT_MIN;
      for (map<int, int>::value_type& kv : weight) {
        if (numbered.find(kv.first) == numbered.end() && kv.second > value) {
          max_unnumbered = kv.first, value = kv.second;
        }
      }
      order[max_unnumbered] = i;
      numbered.emplace(max_unnumbered);

      for (int v : graph.edges(max_unnumbered)) {
        if (!graph.vertices[v].active) continue;
        if (numbered.find(v) == numbered.end()) {
          weight[v]++;
        }
      }
    }
    return order;
  }

  static bool is_chordal (AdjacencyList& graph, map<int, int> order) {
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


  static bool is_chordal (AdjacencyList& graph) {
    map<int, int> ordering = order(graph);
    return is_chordal(graph, ordering);
  }

};