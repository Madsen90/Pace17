#include "LexBFS.h"
#include <list>
#include <set>
#include <vector>
#include <algorithm>
#include <iterator>

LexBFS::LexBFS(int num_vertices)
  : num_vertices(num_vertices),
  ordering(vector<int>(num_vertices)),
  positions(vector<int>(num_vertices)),
  chordal(Chordal::Dirty)
{ }

void LexBFS::order(AdjacencyList& graph) {
  list<set<int>> partitions;
  int label = num_vertices - 1;

  // Inital partition is single set of all vertices
  set<int> initial_partition;
  for (int i = 0; i < graph.num_vertices; i++)
    if (graph.vertices[i].active)
      initial_partition.emplace(i);
  partitions.push_back(initial_partition);

  while (!partitions.empty()) {
    // Pivot is first vertex of first partition
    auto pf = partitions.begin();
    int pivot;
    if (pf->empty()) {
      partitions.pop_front();
      continue;
    }
    pivot = *pf->begin();

    // Pivot's place in ordering found - remove pivot from search
    pf->erase(pivot);
    ordering[label] = pivot;
    positions[pivot] = label;
    label--;

    set<int> adjacency = graph.edges(pivot);
    std::list<set<int>>::iterator pi = partitions.begin();
    while (pi != partitions.end()) {
      set<int> intersection;
      std::set_intersection(
        adjacency.begin(), adjacency.end(),
        pi->begin(), pi->end(),
        inserter(intersection, intersection.begin()));

      if (!intersection.empty() && intersection != *pi) {
        for (int v : intersection)
          pi->erase(v);

        // Insert intersection before pi (it)
        partitions.insert(pi, move(intersection));
      }

      if (pi->empty()) pi = partitions.erase(pi);
      else ++pi;
    }
  }

  chordal = Chordal::Dirty;
}

bool LexBFS::is_chordal(AdjacencyList& graph) {
  if (chordal == Chordal::Dirty) {
    for (int v = 0; v < num_vertices; v++) {
      int order = positions[v];
      set<int> higher_order_neighbours;
      for (int n : graph.edges(v)) {
        if (positions[n] > order) {
          higher_order_neighbours.emplace(n);
        }
      }
      if (!graph.is_clique(higher_order_neighbours)) {
        chordal = Chordal::False;
        return false;
      }
    }
    chordal = Chordal::True;
    return true;
  }
  return chordal == Chordal::True;
}

inline int LexBFS::vertex(int position) {
  return ordering[position];
}

inline int LexBFS::position(int vertex) {
  return positions[vertex];
}