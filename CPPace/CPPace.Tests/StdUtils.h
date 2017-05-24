#pragma once
#include <algorithm>

// this holds std functions, which are not able to be refrenced in visual studios unit tests

using namespace std;

template<typename T>
static T std_min(T a, T b) { return std::min(a, b); }

template<class _RanIt,
  class _Pr> inline
  void std_sort(_RanIt _First, _RanIt _Last, _Pr _Pred)
{	
  sort(_Unchecked(_First), _Unchecked(_Last), _Pred);
}

