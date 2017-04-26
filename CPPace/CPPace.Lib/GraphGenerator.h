#pragma once

#ifdef GRAPHGENERATOR_EXPORTS
#define GRAPHGENERATOR_API _declspec(dllexport)
#else
#define GRAPHGENERATOR_API _declspec(dllimport)
#endif

#include "AdjacencyList.h"

using namespace std;

class GRAPHGENERATOR_API GraphGenerator {
public:
  static AdjacencyList *random(int num_vertices, int num_edges);
};
