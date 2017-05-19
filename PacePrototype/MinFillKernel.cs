using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PacePrototype.wmKernel
{
    public class Triple<T,U,V>
    {
        public T A { get; set; }
        public U B { get; set; }
        public V C { get; set; }

        public Triple(T a, U b, V c)
        {
            A = a;
            B = b;
            C = c;
        }
    }

    public abstract class Optional<T>
    {
        public abstract bool isPresent();
        public abstract T get();
    }

    public class MinFillKernel
    {
        public Triple<HashSet<int>, HashSet<int>, int> kernelProcedure1And2(Graph g)
        {
            HashSet<int> A = new HashSet<int>(), B = g.vertices();
            int kMin = 0;

            // P1
            bool cycleFound;
            do
            {
                cycleFound = false;
                Optional<List<int>> cycle = g.inducedBy(B).findChordlessCycle();
                if (cycle.isPresent())
                {
                    cycleFound = true;
                    HashSet<int> cycleSet = new HashSet<int>(cycle.get());
                    if(! (cycleSet.Count >= 4))
                        throw new Exception("Chordless error in kernelizer");

                    kMin += cycleSet.Count- 3;

                    A.UnionWith(cycleSet);
                    B.ExceptWith(cycleSet);
                }
            } while (cycleFound);

            // P2
            p2:
            do
            {
                cycleFound = false;

                foreach (int u in A)
                {
                    foreach (int x in g.neighborhood(u).toSet().intersect(B))
                    {
                        Graph gPrime = g.inducedBy(g.vertices().remove(x));
                        HashSet<int> R = (g.neighborhood(x).toSet().minus(g.neighborhood(u).toSet())).intersect(B);

                        foreach (int v in R)
                        {
                            if (gPrime.hasPath(u, v))
                            {
                                cycleFound = true;
                                List<int> path = gPrime.shortestPath(u, v);
                                path.Add(x);

                                List<HashSet<int>> subPaths = new List<HashSet<int>>();

                                bool prevInB = false;
                                HashSet<int> subPath = new HashSet<int>();
                                foreach (int vertex in path)
                                {
                                    if (prevInB)
                                    {
                                        if (B.Contains(vertex))
                                        {
                                            subPath.Add(vertex);
                                        }
                                        else
                                        {
                                            subPaths.Add(new HashSet<int>(subPath));
                                            subPath = new HashSet<int>();
                                            prevInB = false;
                                        }
                                    }
                                    else
                                    {
                                        if (B.Contains(vertex))
                                        {
                                            subPath.Add(vertex);
                                            prevInB = true;
                                        }
                                    }
                                }
                                if (subPath.Count != 0) subPaths.Add(new HashSet<int>(subPath));

                                HashSet<int> vertices = new HashSet<int>(path);
                                A.UnionWith(vertices);
                                B.ExceptWith(vertices);

                                subPaths.OrderBy(sub => -sub.Count);

                                if (subPaths.Count == 1)
                                {
                                    if (subPaths[0].Count == path.Count - 1)
                                    {
                                        kMin += subPaths[0].Count - 1;
                                    }
                                    else
                                    {
                                        kMin += subPaths[0].Count - 2;
                                    }
                                }
                                else
                                {
                                    kMin += Math.Max(subPaths.Select(set => set.Count).Sum() / 2, subPaths[0].Count);
                                }

                                continue p2;
                            }
                        }
                    }
                }
            } while (cycleFound);

            return new Triple<HashSet<int>, HashSet<int>, int>(A, B, kMin);
        }
        
        public Optional<Pair<Graph, int>> kernelProcedure3(Graph g, HashSet<int> A, HashSet<int> B, int k)
        {
            int kPrime = k;

            // P3
            foreach (Edge nonEdge in g.inducedBy(A).getNonEdges())
            {
                int x = nonEdge.from, y = nonEdge.to;

                HashSet<int> bNeighbors = g.neighborhood(x).toSet().intersect(g.neighborhood(y).toSet()).intersect(B);
                HashSet<int> Axy = new HashSet<int>();

                foreach (int b in bNeighbors)
                {
                    Graph gPrime = g.inducedBy(g.vertices().remove(b));

                    if (gPrime.hasPath(x, y))
                    {
                        Axy.Add(b);
                    }
                }

                if (Axy.size() > 2 * k)
                {
                    g = g.addEdge(nonEdge);
                    kPrime--;

                    if (kPrime < 0) return Optional.empty();
                }
                else
                {
                    HashSet<int> set = Set.of(Axy);
                    A = A.union(set);
                    B = B.minus(set);
                }
            }

            return Optional.of(Tuple.of(g.inducedBy(A), kPrime));
        }
    }
}
