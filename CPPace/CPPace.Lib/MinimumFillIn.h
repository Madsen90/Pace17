#pragma once
#include "EXPORT.h"
#include "AdjacencyList.h"

#include <vector>
#include <set>
#include <stack>

using namespace std;

class DLLEXPORT MinimumFillIn {
public:
  static bool is_path_chordless(AdjacencyList& graph, vector<int>& path);
  static bool find_four_cycle(AdjacencyList& graph, vector<int>&);
  static bool find_v_star(AdjacencyList& graph, int x, int y, set<int>& moplex, int& v_star);
  static vector<set<int>> find_moplexes(AdjacencyList& graph);
  static stack<pair<int, int>> minimum_fill_in(AdjacencyList& graph);
};
