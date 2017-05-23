#pragma once
#include "EXPORT.h"


#include "AdjacencyList.h"
#include <set>
#include <vector>
#include <map>

using namespace std;

struct Kernel
{
  set<int> a;
  set<int> b;
  int kMin;
  set<pair<int, int>> essential_edges;
};

class DLLEXPORT Kernelizer {
public:
  static Kernel phase1(AdjacencyList& graph);
  static bool phase2(AdjacencyList& graph, Kernel& phase1_kernel);
  static bool Kernelizer::phase3(AdjacencyList& graph, Kernel& phase2_kernel, Kernel& phase3_kernel, int max_k);

//private:
  static bool find_chordless_cycle(AdjacencyList& graph, vector<int>& cycle);
  static vector<pair<int, int>> find_non_edges(AdjacencyList& graph);
  static map<int, int> MCS(AdjacencyList& graph);
  static bool BFS_path(AdjacencyList& graph, int start, int end, vector<int>& path);

};
