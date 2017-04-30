#pragma once

#ifdef GRAPHIO_EXPORTS
#define GRAPHIO_API _declspec(dllexport)
#else
#define GRAPHIO_API _declspec(dllimport)
#endif

#include <iostream>
#include "AdjacencyList.h"

using namespace std;

class GRAPHIO_API GraphIO {
public:
  enum Format { EdgeList, DOT };
  static AdjacencyList read_from_stream(istream& stream);
  static AdjacencyList read_from_path(string path);
  static void write_to_stream(AdjacencyList& graph, ostream& stream, Format format = Format::EdgeList);
  static void write_to_path(AdjacencyList& graph, string path, Format format = Format::EdgeList);
};
