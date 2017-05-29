#pragma once
#include "EXPORT.h"

#include <vector>
#include <set>
#include <stack>
#include "AdjacencyList.h"
#include "GraphIO.h"

using namespace std;
class Terminator
{
public:
  bool terminate = false;
};


class DLLEXPORT MinimumFillIn {
public:
  static bool is_path_chordless(AdjacencyList& graph, vector<int>& path);
  static bool find_four_cycle(AdjacencyList& graph, vector<int>&);
  static bool find_v_star(AdjacencyList& graph, int x, int y, set<int>& moplex, int& v_star);
  static vector<set<int>> find_moplexes(AdjacencyList& graph, vector<set<int>>& previous_moplexes, set<pair<int, int>>& newly_added_edges);
  static stack<pair<int, int>> minimum_fill_in(GraphIO::GraphContext context, Terminator* t);
};
