#pragma once
#include "EXPORT.h"

#include <vector>
#include <set>
#include "AdjacencyList.h"

using namespace std;

class DLLEXPORT MinimumFillIn {
public:
  static bool is_path_chordless(AdjacencyList& graph, vector<int>& path);
  static bool find_four_cycle(AdjacencyList& graph, vector<int>&);
  static bool find_v_star(AdjacencyList& graph, int x, int y, set<int>& moplex, int& v_star);
};
