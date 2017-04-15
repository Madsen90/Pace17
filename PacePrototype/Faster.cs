using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using QuickGraph;
using QuickGraph.Graphviz.Dot;

namespace PacePrototype
{
    class Faster
    {
        public static int Run(UndirectedGraph<int, Edge<int>> graph)
        {
            int ret = -1;
            int k = 4;
            while (ret == -1)
            {
                k++;
                ret = FasterInner(graph, k, k * 2, new HashSet<int>());
            }


            return k - ret;
        }

        private static int FasterInner(UndirectedGraph<int, Edge<int>> graph, int k, int r, HashSet<int> Marked)
        {
            // Trivial cases
            if (k < 0 || r < -1)
                return -1;

            var analysis = MoplexAnalysis.AnalyseGraph(graph);
            if (IsChordal(analysis))
                return k;

            // Find four cycle
            List<int> cycle = FindFourCycle(graph);
            if(cycle != null)
            {
                var graph1 = CloneGraph(graph);
                var graph2 = CloneGraph(graph);
                graph1.AddEdge(new Edge<int>(cycle[0], cycle[3]));
                graph2.AddEdge(new Edge<int>(cycle[1], cycle[2]));
                var Marked1 = new HashSet<int>(Marked);
                var Marked2 = new HashSet<int>(Marked);
                var r1 = r;
                var r2 = r;

                if (Marked.Contains(cycle[0]))
                    Marked1.Remove(cycle[0]);
                else
                    r1--;
                if (Marked.Contains(cycle[3]))
                    Marked1.Remove(cycle[3]);
                else
                    r1--;
                if (Marked.Contains(cycle[1]))
                    Marked2.Remove(cycle[1]);
                else
                    r2--;
                if (Marked.Contains(cycle[2]))
                    Marked2.Remove(cycle[2]);
                else
                    r2--;
                return Math.Max(FasterInner(graph1, k - 1, r1, Marked1), FasterInner(graph2, k - 1, r2, Marked2));
            }

            //find moplex with both marked and unmarked edges
            List<List<int>> markedAndUnmarked = analysis.Moplexes.Where(vl => vl.Exists(v => Marked.Contains(v))).Where(vl => vl.Exists(v => !Marked.Contains(v))).ToList();
            
            if (markedAndUnmarked.Count > 0)
            {
                foreach (var moplex in markedAndUnmarked)
                {
                    foreach (var vertex in moplex)
                    {
                        var previousSize = Marked.Count;
                        Marked.Add(vertex);
                        if (previousSize < Marked.Count)
                            r--;
                    }
                }
                return FasterInner(graph, k, r, Marked);
            }

            //simplicial moplex with only unmarked vertices
            List<List<int>> unmarkedMoplexes = analysis.Moplexes.Where(vl => vl.TrueForAll(v => !Marked.Contains(v))).ToList();
            List<List<int>> simplicialUnmarked = unmarkedMoplexes.Where(vl =>
            {
                var neighbourhood = new HashSet<int>();

                foreach (var v in vl)
                {
                    foreach (var e in graph.AdjacentEdges(v))
                    {
                        neighbourhood.Add(e.GetOtherVertex(v));
                    }
                }

                return IsClique(neighbourhood, analysis.EleminationOrder, analysis.NeighbourLabels);
            }).ToList();

            if(simplicialUnmarked.Count > 0)
            {
                foreach(var neighbourhood in simplicialUnmarked)
                {
                    foreach (var v in neighbourhood)
                    {
                        graph.RemoveVertex(v);
                    }
                }
                return FasterInner(graph, k, r, Marked);
            }

            // Moplex with only unmarked vertices and neighbourhood only missing one edge
            Edge<int> missingEdge = null;
            HashSet<int> moplexNeighbourhood = null;
            int m = -1;
            foreach (var moplex in unmarkedMoplexes)
            {
                m++;
                moplexNeighbourhood = new HashSet<int>(moplex.SelectMany(v => graph.AdjacentEdges(v).Select(e => e.GetOtherVertex(v)))); //this includes the moplex - perhaps it shouldn't
                foreach (var v in moplex)
                {
                    moplexNeighbourhood.Remove(v);
                }
                var missingEdges = MissingEdges(moplexNeighbourhood, analysis.NeighbourLabels);
                if(missingEdges.Count == 1)
                {
                    missingEdge = missingEdges.First();
                    break;
                }
            }
            if(missingEdge != null)
            {
                HashSet<int> moplexNeighbourhoodMarked = CloneSet(moplexNeighbourhood);
                moplexNeighbourhoodMarked.RemoveWhere(v => !Marked.Contains(v));

                var vStar = FindVStar(missingEdge, moplexNeighbourhoodMarked, graph);
                if(vStar == -1)
                {
                    vStar = FindVStar(new Edge<int>(missingEdge.Target, missingEdge.Source), moplexNeighbourhoodMarked, graph);
                }
                if (Marked.Contains(missingEdge.Source))
                {
                    Marked.Remove(missingEdge.Source);
                }
                else r--;
                if (Marked.Contains(missingEdge.Target))
                {
                    Marked.Remove(missingEdge.Target);
                }
                else r--;
                graph.AddEdge(missingEdge);
                return FasterInner(graph, k - 1, r, Marked);
            }

            var markedMoplexes = analysis.Moplexes.Where(vl => vl.TrueForAll(v => Marked.Contains(v))).ToList();
            if(markedMoplexes.Count == analysis.Moplexes.Count)
                return -1;
            
            if(unmarkedMoplexes.Count > 0)
            {
                var moplex = unmarkedMoplexes.First();
                //branch 1:
                var Marked1 = new HashSet<int>(Marked);
                moplex.ForEach(v => Marked1.Add(v));
                var r1 = r - (Marked1.Count - Marked.Count);
                var b1 = FasterInner(graph, k, r1, Marked1);

                //branch 2:
                var graph2 = CloneGraph(graph);
                var neighbourhood = new HashSet<int>();
                foreach (var v in moplex)
                {
                    var vAdj = graph.AdjacentEdges(v);
                    foreach (var e in vAdj)
                    {
                        neighbourhood.Add(e.GetOtherVertex(v));
                    }
                }
                var missingEdges = MissingEdges(neighbourhood, analysis.NeighbourLabels);
                graph2.AddEdgeRange(missingEdges);
                var k2 = k - (missingEdges.Count);
                var b2 = FasterInner(graph2, k2, r, Marked);
                return Math.Max(b1, b2);
                
            }



            return -1;
        }

        private static HashSet<int> CloneSet(HashSet<int> org)
        {
            var clone = new HashSet<int>();
            foreach (var i in org)
            {
                clone.Add(i);
            }
            return clone;
            
        }

        public static int FindVStar(Edge<int> missingEdge, HashSet<int> neighbourhood, UndirectedGraph<int, Edge<int>> graph)
        {
            var x = missingEdge.Source;
            var y = missingEdge.Target;
            var component = new UndirectedGraph<int, Edge<int>>();
            var visited = new HashSet<int> { x };
            var complete = new List<List<int>>();
            component.AddVertexRange(neighbourhood);
            foreach (var v in neighbourhood)
            {
                component.AddEdgeRange(graph.AdjacentEdges(v)); //adds edges not in component, however, since no target vertices will exist if not in the component, these edges will be ignored by QuickGraph
            }
            Queue<List<int>> q = new Queue<List<int>>();
            foreach (var e in component.AdjacentEdges(x))
            {
                var n = e.GetOtherVertex(x);
                var l = new List<int> { x, n };
                q.Enqueue(l);
            }
            while(q.Count > 0)
            {
                var l = q.Dequeue();
                var n = l.Last();
                if (visited.Contains(n)) // What if they have same iteration number?
                {
                    continue; // l is not cordless.
                }
                if(n == y)
                {
                    complete.Add(l); // The coordless path is complete 
                    continue;
                }
                visited.Add(n);

                foreach(var e in graph.AdjacentEdges(n))
                {
                    var v = e.GetOtherVertex(n);
                    if (visited.Contains(v))
                        continue;
                    var l2 = CloneList(l);
                    l2.Add(v);
                    q.Enqueue(l2);
                }

            }
            complete.ForEach(l => l.Remove(y));
            List<int> vStars = complete.Select(l => l.Last()).ToList();
            int vStar = vStars.First();
            if (vStars.TrueForAll(v => v == vStar))
            {
                return vStar;
            }

            return -1;
            
        }

        private static List<int> CloneList(List<int> l)
        {
            var clone = new List<int>();
            foreach (var i in l)
            {
                clone.Add(i);
            }
            return clone;
        }

        private static UndirectedGraph<int, Edge<int>> CloneGraph(UndirectedGraph<int, Edge<int>> graph)
        {
            var clone = new UndirectedGraph<int, Edge<int>>();
            foreach (var v in graph.Vertices)
            {
                clone.AddVertex(v);
            }
            foreach (var e in graph.Edges)
            {
                clone.AddEdge(new Edge<int>(e.Source, e.Target));
            }
            return clone;
        }

        // Dumb n^4 four-cycle finder
        public static List<int> FindFourCycle(UndirectedGraph<int, Edge<int>> graph)
        {
            foreach (var v in graph.Vertices)
            {
                var vAdj = graph.AdjacentEdges(v).ToList();
                for (int i = 0; i < vAdj.Count; i++)
                {
                    for (int j = 0; j < vAdj.Count; j++)
                    {
                        if (i == j)
                            continue;
                        var vi = vAdj[i].Source == v ? vAdj[i].Target : vAdj[i].Source;
                        var vj = vAdj[j].Source == v ? vAdj[j].Target : vAdj[j].Source;

                        Edge<int> connectingEdge;
                        graph.TryGetEdge(vi, vj, out connectingEdge);
                        if(connectingEdge == null)
                            graph.TryGetEdge(vj, vi, out connectingEdge);
                        if (connectingEdge != null) //three cycle
                            continue;

                        var viAdj = new HashSet<int>(graph.AdjacentEdges(vi).Select(e => e.GetOtherVertex(vi)));
                        var vjAdj = new HashSet<int>(graph.AdjacentEdges(vj).Select(e => e.GetOtherVertex(vj)));
                        var common = new HashSet<int>(viAdj.Intersect(vjAdj));
                        common.Remove(v);
                        if (common.Count > 0)
                            return new List<int> { v, vi, vj, common.First() };
                        
                    }
                }
            }
            return null;
        }


        // These two (ischordal + isclique) could probably be done a lot faster and better. See https://github.com/omid69/PEO-Verification
        public static bool IsChordal(MoplexAnalysis analysis)
        {
            var ordering = analysis.EleminationOrder;
            var labels = analysis.NeighbourLabels;

            for (int i = ordering.Length -1; i >= 0; i--)
            {
                int v = ordering[i];
                var clique = new HashSet<int>(labels[i].Where(j => j > v).Select(label => analysis.EleminationOrderRev[label]));
                
                clique.Add(i);
                if (!IsClique(clique, analysis.EleminationOrder,labels))
                    return false;
            }

            return true;
        }

        private static List<Edge<int>> MissingEdges(HashSet<int> clique, List<int>[] labels)
        {
            var missingEdges = new List<Edge<int>>();
            foreach (var vertex in clique)
            {
                foreach (var neighbour in clique)
                {
                    if (vertex == neighbour)
                        continue;
                    if (!labels[neighbour].Contains(vertex))
                        missingEdges.Add(new Edge<int>(Math.Min(vertex, neighbour), Math.Max(vertex, neighbour)));
                }
            }
            return missingEdges;
        }

        private static bool IsClique(HashSet<int> clique, int[] order, List<int>[] labels)
        {
            foreach (var vertex in clique)
            {
                foreach(var neighbour in clique)
                {
                    if (vertex == neighbour)
                        continue;
                    if (!labels[neighbour].Contains(order[vertex]))
                        return false;
                }
            }
            return true;
        }
    }

}
