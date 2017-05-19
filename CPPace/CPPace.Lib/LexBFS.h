#pragma once
#include "EXPORT.h"

#include "AdjacencyList.h"

using namespace std;

class DLLEXPORT LexBFS {
public:
  int num_vertices;

  LexBFS(int num_vertices);

  void order(AdjacencyList& graph);
  bool is_chordal(AdjacencyList& graph);
  int vertex(int position);
  int position(int vertex);

private:
  // Array of vertices in lexicographic order
  int* ordering;

  // The inverse of ordering - positions[v] is vertex v's position in ordering
  int* positions;

  // Allows lazy chordality checking
  enum Chordal { Dirty, True, False };
  Chordal chordal;
};
