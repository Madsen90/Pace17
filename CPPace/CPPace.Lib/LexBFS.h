#pragma once

#ifdef LEXBFS_EXPORTS
#define LEXBFS_API _declspec(dllexport)
#else
#define LEXBFS_API _declspec(dllimport)
#endif

#include "AdjacencyList.h"

using namespace std;

class LEXBFS_API LexBFS {
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
