#include "GraphIO.h"
#include <vector>
#include <fstream>
#include <algorithm>

AdjacencyList GraphIO::read_from_stream(istream& stream) {
  vector<int> us, vs;
  int u, v;
  while (stream >> u >> v) {
    us.emplace_back(u);
    vs.emplace_back(v);
  }

  int num_vertices = 0;
  for (int i = 0; i < us.size(); i++)
    num_vertices = max(num_vertices, max(us[i], vs[i]));
  num_vertices++; // Vertices are 0-indexed

  AdjacencyList graph(num_vertices);
  for (int i = 0; i < us.size(); i++)
    graph.add_edge(us[i], vs[i]);

  return graph;
}

AdjacencyList GraphIO::read_from_path(string path) {
  ifstream infile(path);
  return read_from_stream(infile);
}

void GraphIO::write_to_stream(AdjacencyList& graph, ostream& stream, Format format) {
  vector<pair<int, int>> edges = graph.all_edges();
  switch (format) {
  case Format::EdgeList:
    for (auto edge : edges)
      stream << edge.first << " " << edge.second << endl;
    break;
  case Format::DOT:
    stream << "graph {" << endl;
    for (pair<int, int> edge : graph.all_edges())
      stream << "\t" << edge.first << " -- " << edge.second << ";" << endl;
    stream << "}" << endl;
    break;
  }
}

void GraphIO::write_to_path(AdjacencyList& graph, string path, Format format) {
  ofstream outfile(path);
  write_to_stream(graph, outfile, format);
}
