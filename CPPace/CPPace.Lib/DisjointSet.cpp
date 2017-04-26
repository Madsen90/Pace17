#include "DisjointSet.h"

DisjointSet::DisjointSet(int size)
  : size(size)
{
  parents = new int[size];
  ranks = new int[size] {};

  for (int i = 0; i < size; ++i)
    parents[i] = i;
}

int DisjointSet::find(int i)
{
  int p = parents[i];
  if (p != i)
    p = parents[i] = find(p);

  return p;
}

void DisjointSet::merge(int i, int j)
{
  auto iroot = find(i);
  auto jroot = find(j);

  if (iroot == jroot) return;

  auto irank = ranks[iroot];
  auto jrank = ranks[jroot];

  if      (irank > jrank) parents[jroot] = iroot;
  else if (irank < jrank) parents[iroot] = jroot;
  else {
    parents[jroot] = iroot;
    ranks[iroot]++;
  }
}
