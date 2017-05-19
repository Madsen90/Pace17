#pragma once
#include "EXPORT.h"

struct DLLEXPORT DisjointSet
{
public:
  int size;
  int *parents;
  int *ranks;

  DisjointSet(int size);

  int find(int i);
  void merge(int i, int j);
};
