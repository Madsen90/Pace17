#include "DotConvert.h"

void DotConvert::write_to_stream(AdjacencyList graph, ostream& stream) {
  stream << "graph {" << endl;

  for (int u = 0; u < graph.num_vertices; u++) {
    set<int> edges = graph.edges(u);
    for (int v : edges) {
      if (v < u) continue;
      stream << "\t" << u << " -- " << v << ";" << endl;
    }
  }

  stream << "}" << endl;
}
