#include "LexBFS.h"
#include <list>
#include <set>
#include <vector>
#include <algorithm>
#include <iterator>

LexBFS::LexBFS(int num_vertices)
  : num_vertices(num_vertices),
    ordering(new int[num_vertices * 2]),
    positions(new int[num_vertices * 2]),
    chordal(Chordal::Dirty)
{ }

void LexBFS::order(AdjacencyList& graph) {
  list<set<int>*> partitions;
  int label = 0;
  
  // Inital partition is single set of all vertices
  set<int>* initial_partition = new set<int>();
  for (int i = 0; i < graph.num_vertices; i++)
    initial_partition->emplace(i);
  partitions.push_back(initial_partition);

  while (!partitions.empty()) {
    // Pivot is first vertex of first partition
    set<int>* pf = partitions.front();
    int pivot = *pf->begin();

    // Pivot's place in ordering found - remove pivot from search
    pf->erase(pivot);
    ordering[label] = pivot;
    positions[pivot] = label;
    label++;

    set<int> adjacency = graph.edges(pivot);
    std::list<set<int>*>::iterator it = partitions.begin();
    while (it != partitions.end()) {
      set<int>* pi = *it;
      set<int>* intersection = new set<int>();
      std::set_intersection(
        adjacency.begin(), adjacency.end(),
        pi->begin(), pi->end(),
        inserter(*intersection, intersection->begin()));

      if (!intersection->empty() && *intersection != *pi) {
        for (int v : *intersection)
          pi->erase(v);

        // Insert intersection before pi (it)
        partitions.insert(it, intersection);
      }

      if (pi->empty()) it = partitions.erase(it);
      else ++it;
    }
  }

  chordal = Chordal::Dirty;
}

bool LexBFS::is_chordal(AdjacencyList& graph) {
  if (chordal == Chordal::Dirty) {
    set<int> visited_vertices;
    AdjacencyList visited_graph(graph.num_vertices);
    for (int i = 0; i < num_vertices; i++) {
      int u = ordering[i];
      set<int> adjacency = graph.edges(u);
      for (int v : adjacency) {
        if (visited_vertices.find(v) != visited_vertices.end())
          visited_graph.add_edge(u, v);
      }
      
      if (!graph.clique(adjacency)) {
        chordal = Chordal::False;
        return false;
      }

      visited_vertices.emplace(u);
    }

    chordal = Chordal::True;
  }

  return chordal == Chordal::True;
}

inline int LexBFS::vertex(int position) {
  return positions[position];
}

inline int LexBFS::position(int vertex) {
  return ordering[vertex];
}