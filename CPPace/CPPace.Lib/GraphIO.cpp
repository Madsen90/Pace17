#include "GraphIO.h"
#include <vector>
#include <fstream>
#include <algorithm>
#include <string>
#include <unordered_map>

GraphContext GraphIO::read_from_stream(istream& stream) {
  int index = 0;
  unordered_map<string, int> name_to_index;
  vector<int> us, vs;
  string u_name, v_name;
  while (stream >> u_name >> v_name) {
    if (name_to_index.find(u_name) == name_to_index.end()) name_to_index[u_name] = index++;
    if (name_to_index.find(v_name) == name_to_index.end()) name_to_index[v_name] = index++;
    us.emplace_back(name_to_index[u_name]);
    vs.emplace_back(name_to_index[v_name]);
  }

  AdjacencyList graph(index);
  for (int i = 0; i < us.size(); i++)
    graph.add_edge(us[i], vs[i]);

  vector<string> vertex_names(index);
  for (pair<string, int> kv : name_to_index)
    vertex_names[kv.second] = kv.first;

  return GraphContext(graph, vertex_names);
}

GraphContext GraphIO::read_from_path(string path) {
  ifstream infile(path);
  return read_from_stream(infile);
}

void GraphIO::write_to_stream(GraphContext& context, ostream& stream, Format format) {
  vector<pair<int, int>> edges = context.graph.all_edges();
  switch (format) {
  case Format::EdgeList:
    for (auto edge : edges)
      stream << context.vertex_name(edge.first)
             << " "
             << context.vertex_name(edge.second)
             << endl;
    break;
  case Format::DOT:
    stream << "graph {" << endl;
    for (pair<int, int> edge : context.graph.all_edges())
      stream << "\t"
             << context.vertex_name(edge.first)
             << " -- "
             << context.vertex_name(edge.second)
             << ";"
             << endl;
    stream << "}" << endl;
    break;
  }
}

void GraphIO::write_to_path(GraphContext& graph, string path, Format format) {
  ofstream outfile(path);
  write_to_stream(graph, outfile, format);
}
