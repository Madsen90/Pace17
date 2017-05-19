#pragma once

#ifdef GRAPHIO_EXPORTS
#define GRAPHIO_API _declspec(dllexport)
#else
#define GRAPHIO_API _declspec(dllimport)
#endif

#include <iostream>
#include <string>
#include <unordered_map>
#include "AdjacencyList.h"

using namespace std;

class GRAPHIO_API GraphIO {
public:
  class GraphContext {
  public:
    AdjacencyList graph;
    vector<string> vertex_names;
    unordered_map<string, set<pair<int, int>>> edge_color_map;
    unordered_map<string, set<int>> vertex_color_map;

    GraphContext(AdjacencyList& graph, vector<string>& vertex_names, unordered_map<string, set<pair<int, int>>>& edge_color_map, unordered_map<string, set<int>> vertex_color_map)
      : graph(graph),
        vertex_names(vertex_names),
        edge_color_map(edge_color_map),
        vertex_color_map(vertex_color_map)
    { }

    GraphContext(AdjacencyList& graph)
      : graph(graph),
        vertex_names(),
        edge_color_map(),
        vertex_color_map()
    {
      for (int u = 0; u < graph.num_vertices; u++)
        vertex_names.emplace_back(to_string(u));
    }

    string vertex_name(int vertex) {
      return vertex_names[vertex];
    }
  };

  // EdgeList    Every line describes an edge as "<u> <v>"
  // DOT         Follows the DOT-format
  enum Format { EdgeList, DOT };
  static GraphContext read_from_stream(istream& stream);
  static GraphContext read_from_path(string path);
  static void write_to_stream(GraphContext& graph, ostream& stream, Format format = Format::EdgeList);
  static void write_to_path(GraphContext& graph, string path, Format format = Format::EdgeList);
};
