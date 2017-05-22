#pragma once
#include "AdjacencyList.h"
#include <set>
#include <vector>

using namespace std;

struct Kernel
{
  set<int> a;
  set<int> b;
  int chordless_cycles;
  set<pair<int, int>> essential_edges;
};

class Kernelizer {
public:
  static Kernel phase1(AdjacencyList& graph);
  static Kernel phase2(AdjacencyList& graph, Kernel& phase1_kernel);
  static Kernel phase3(AdjacencyList& graph, Kernel& phase1_kernel, int min_k);

private:
  static bool find_chordless_cycle(AdjacencyList& graph, vector<int>& cycle);
  static vector<pair<int, int>> find_non_edges(AdjacencyList& graph);
  static vector<int> MCS(AdjacencyList& graph);
  static bool BFS_path(AdjacencyList& graph, int start, int end, vector<int>& path);

};
