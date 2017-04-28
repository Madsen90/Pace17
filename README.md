# Pace17

# How to find all moplexes
Create an empty list, mop, to hold all the moplexes.
Create an empty list, lm, representing the vertex set that is currently being checked for moplexity.
Create an empty list, lp, representing the vertex set that has already been checked for moplexity.

For each vertex v in graph g do these 7 steps:

1. If v is in lp, skip this iteration, v has already been checked.
Add v to lm.

2. For each of v's neighbours, vn, check if N[v] = N[vn], where N[v] is v's neighbour set v including v.
If so, add vn to lm.

3. Take an arbitray vertex, w, from lm. Use w to get the neighbour set, ln, of the potential moplex, N[w] / lm. 
Since all vertices in lm share a neighbourhood, w can be chosen arbitrarily from lm.

4. Remove lm and ln from g to produce g'.

5. Use BFS to find the connected components of g', denoted cc.

6. Check that each vertex in ln is connected to every component in cc. 
If this true for every vertex in ln, add lm to mop.

7. Add every vertex in lm to lp.
Reiterate