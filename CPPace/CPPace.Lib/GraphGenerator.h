#pragma once
#include "EXPORT.h"

#include "AdjacencyList.h"

using namespace std;

class DLLEXPORT GraphGenerator {
public:
  static AdjacencyList random(int num_vertices, int num_edges);
};
