#pragma once

#ifdef GRAPHIO_EXPORTS
#define GRAPHIO_API _declspec(dllexport)
#else
#define GRAPHIO_API _declspec(dllimport)
#endif

#include <iostream>
#include "AdjacencyList.h"

using namespace std;

struct GraphContext {
public:
  AdjacencyList graph;

  GraphContext(AdjacencyList graph, vector<string> vertex_names)
    : graph(graph),
      vertex_names(vertex_names)
  { }

  string vertex_name(int vertex) {
    return vertex_names[vertex];
  }

private:
  vector<string> vertex_names;
};

class GRAPHIO_API GraphIO {
public:
  // EdgeList    Every line describes an edge as "<u> <v>"
  // DOT         Follows the DOT-format
  enum Format { EdgeList, DOT };
  static GraphContext read_from_stream(istream& stream);
  static GraphContext read_from_path(string path);
  static void write_to_stream(GraphContext& graph, ostream& stream, Format format = Format::EdgeList);
  static void write_to_path(GraphContext& graph, string path, Format format = Format::EdgeList);
};
