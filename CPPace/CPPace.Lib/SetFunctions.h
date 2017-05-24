#pragma once
#include "EXPORT.h"

#include <set>
#include <vector>

using namespace std;

class DLLEXPORT SetFunctions {
public:
  static set<int> set_union_two(set<int>& s1, set<int>& s2);

  static set<int> set_intersect_two(set<int>& s1, set<int>& s2);
  static set<int> set_intersect_three(set<int>& s1, set<int>& s2, set<int>& s3);
};
