#pragma once
#include "EXPORT.h"

#include "AdjacencyList.h"

using namespace std;

class DLLEXPORT GraphGenerator {
public:
  static AdjacencyList random(int num_vertices, int num_edges);
  static AdjacencyList fully_connected(int n);
  static AdjacencyList half_connected(int n);
  static AdjacencyList cycle(int n);
};
