#pragma once

#ifdef DISJOINTSET_EXPORTS
#define DISJOINTSET_API _declspec(dllexport)
#else
#define DISJOINTSET_API _declspec(dllimport)
#endif

struct DISJOINTSET_API DisjointSet
{
public:
  int size;
  int *parents;
  int *ranks;

  DisjointSet(int size);

  int find(int i);
  void merge(int i, int j);
};
