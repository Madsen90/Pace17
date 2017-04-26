using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using QuickGraph;
using QuickGraph.Graphviz.Dot;
using QuickGraph.Graphviz;
using System.IO;

namespace PacePrototype
{
    class Faster
    {

        public static (int, HashSet<Edge<int>>) Run(UndirectedGraph<int, Edge<int>> graph)
        {
            var componentAlgorithm = new QuickGraph.Algorithms.ConnectedComponents.ConnectedComponentsAlgorithm<int, Edge<int>>(graph);
            componentAlgorithm.Compute();
            var nodeToComponent = componentAlgorithm.Components;
            var time = DateTime.Now;
            var retk = 0;
            var retEdges = new HashSet<Edge<int>>();
            for(int i = 0; i < componentAlgorithm.ComponentCount; i++)
            {
                var g = new UndirectedGraph<int, Edge<int>>();
                foreach (var v in graph.Vertices)
                {
                    if(nodeToComponent[v] == i)
                    {
                        g.AddVertex(v);
                    }
                }
                foreach(var v in g.Vertices)
                {
                    foreach(var e in graph.AdjacentEdges(v))
                    {
                        if (g.ContainsVertex(e.GetOtherVertex(v))) g.AddEdge(e);
                    }
                }
                (var k, var edges) = FindK(g, time);
                retk += k;
                foreach (var e in edges) retEdges.Add(e);
            }
            return (retk, retEdges);
        }


        public static (int, HashSet<Edge<int>>) FindK(UndirectedGraph<int, Edge<int>> graph, DateTime timeOfInit)
        {
            int ret = -1;
            int k = -1;
            UndirectedGraph<int, Edge<int>> retGraph = null;
            while (ret == -1)
            {
                var time1 = DateTime.Now;
                var clone = CloneGraph(graph);
                Console.WriteLine(++k);
                (int ret1, UndirectedGraph<int, Edge<int>> graph1) = FasterInner(clone, k, k * 2, new HashSet<int>(), null, null);
                ret = ret1;
                retGraph = graph1;
                Console.WriteLine($"Took {(DateTime.Now - time1).ToString("c")}");
                Console.WriteLine($"Cumulated {(DateTime.Now - timeOfInit).ToString("c")}");
            }
            var edgeSet = new HashSet<Edge<int>>(retGraph.Edges.Where(e => !graph.ContainsEdge(e.Source, e.Target)));
            drawGraph(retGraph, edgeSet, @"C:\Users\Frederik\Desktop\a.dot");
            
            return (k - ret, edgeSet);
        }



        private static void drawGraph(UndirectedGraph<int, Edge<int>> retGraph, HashSet<Edge<int>> edgeSet, string path)
        {
            var a = new UndirectedGraph<int, Edge<int>>();
            var b = retGraph.Vertices.ToList();
            b.Sort();
            a.AddVertexRange(b);
            a.AddEdgeRange(retGraph.Edges);
            var c = new GraphvizAlgorithm<int, Edge<int>>(a);
            c.Generate(new FileDotEngine(edgeSet), path);
        }

        public static (int, UndirectedGraph<int, Edge<int>>) FasterInner(
            UndirectedGraph<int, Edge<int>> graph,
            int k,
            int r,
            HashSet<int> Marked,
            List<Edge<int>> newlyAddedEdges,
            List<List<int>> prevMoplexes
            )
        {
            // Trivial cases
            if (k < 0 || r < -1)
                return (-1, graph);

            var analysis = MoplexAnalysis.AnalyseGraph(graph, newlyAddedEdges, prevMoplexes);
            if (IsChordal2(analysis, graph))
                return (k, graph);

            // Find four cycle
            List<int> cycle = FindFourCycle3(graph); //has to return topological four cycle
            if(cycle != null)
            {
                //var graph1 = CloneGraph(graph); // maybe only clone once
                var clone = CloneGraph(graph);
                var newEdge1 = new Edge<int>(cycle[0], cycle[3]);
                var newEdge2 = new Edge<int>(cycle[1], cycle[2]);
                graph.AddEdge(newEdge1);
                clone.AddEdge(newEdge2);
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
                var (k1, g1) = FasterInner(graph, k - 1, r1, Marked1, new List<Edge<int>> { newEdge1 }, analysis.Moplexes);
                var (k2, g2) = FasterInner(clone, k - 1, r2, Marked2, new List<Edge<int>> { newEdge2 }, analysis.Moplexes);
                if (k1 > k2)
                    return (k1, g1);
                return (k2, g2);
            }

            //find moplex with both marked and unmarked edges
            List<List<int>> markedAndUnmarked = analysis.Moplexes.Where(vl => vl.Exists(v => Marked.Contains(v))).Where(vl => vl.Exists(v => !Marked.Contains(v))).ToList();
            
            if (markedAndUnmarked.Count > 0)
            {
                foreach (var moplex in markedAndUnmarked)
                {
                    foreach (var vertex in moplex)
                    {
                        if (Marked.Add(vertex))
                            r--;
                    }
                }
                return FasterInner(graph, k, r, Marked, null, analysis.Moplexes);
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
                return FasterInner(graph, k, r, Marked, null, analysis.Moplexes.Except(simplicialUnmarked).ToList());
            }

            // Moplex with only unmarked vertices and neighbourhood only missing one edge
            Edge<int> missingEdge = null;
            HashSet<int> moplexNeighbourhood = null;
            int m = -1;
            foreach (var moplex in unmarkedMoplexes)
            {
                m++;
                moplexNeighbourhood = new HashSet<int>(moplex.SelectMany(v => graph.AdjacentEdges(v).Select(e => e.GetOtherVertex(v)))); 
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
                //moplexNeighbourhoodMarked.RemoveWhere(v => !Marked.Contains(v));

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
                return FasterInner(graph, k - 1, r, Marked, new List<Edge<int>> { missingEdge }, analysis.Moplexes);
            }

            var markedMoplexes = analysis.Moplexes.Where(vl => vl.TrueForAll(v => Marked.Contains(v))).ToList();
            if(markedMoplexes.Count == analysis.Moplexes.Count)
                return (-1, null);
            
            if(unmarkedMoplexes.Count > 0)
            {
                var moplex = unmarkedMoplexes.First();
                //branch 1:
                var Marked1 = new HashSet<int>(Marked);
                moplex.ForEach(v => Marked1.Add(v));
                var r1 = r - (Marked1.Count - Marked.Count);
                var (b1, g1) = FasterInner(graph, k, r1, Marked1, null, analysis.Moplexes);

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
                var (b2, g2) = FasterInner(graph2, k2, r, Marked, missingEdges, analysis.Moplexes);

                if (b1 > -1 && b1 < b2)
                    return (b1, g1);
                return (b2, g2);
                
            }



            return (-1, null);
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
                component.AddEdgeRange(graph.AdjacentEdges(v).Where(e => component.ContainsVertex(e.GetOtherVertex(v)))); //adds edges not in component, however, since no target vertices will exist if not in the component, these edges will be ignored by QuickGraph
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
            if (vStars.Any())
            {
                int vStar = vStars.First();
                if (vStars.TrueForAll(v => v == vStar))
                {
                    return vStar;
                }
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
        public static List<int> FindFourCycle1(UndirectedGraph<int, Edge<int>> graph)
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


        // More memory intensive (and faster!!) cycle finder
        public static List<int> FindFourCycle2(UndirectedGraph<int, Edge<int>> graph)
        {
            var idTable = new int[graph.VertexCount];
            var idTableRev = new Dictionary<int, int>();
            int i = 0;
            foreach(int v in graph.Vertices)
            {
                idTable[i] = v;
                idTableRev[v] = i++;
            }
            var matrix = new int[graph.VertexCount, graph.VertexCount];
            matrix.Initialize();
            for (i = 0; i < graph.VertexCount; i++) //Assumes that vertex set is of format {0, 1, ... n-1}
            {
                foreach(var e1 in graph.AdjacentEdges(idTable[i]))
                {
                    var n1 = e1.GetOtherVertex(idTable[i]);
                    foreach (var e2 in graph.AdjacentEdges(idTable[i]))
                    {
                        var n2 = e2.GetOtherVertex(idTable[i]);
                        if (n1 == n2)
                            continue;
                        var temp = Math.Max(n1, n2);
                        n1 = Math.Min(n1, n2);
                        n2 = temp;
                        if (matrix[idTableRev[n1], idTableRev[n2]] == 1 )
                        {
                            if (graph.ContainsEdge(n1, n2))
                                continue;
                            // find the vertice that set matrix[n1, n2] == 1
                            for (int j = 0; j < i; j++)
                            {
                                if(graph.ContainsEdge(idTable[j],n1) && graph.ContainsEdge(idTable[j], n2))
                                {
                                    if (graph.ContainsEdge(idTable[i], idTable[j]))
                                        continue;
                                    return new List<int> { n1, idTable[j], idTable[i], n2 };
                                }
                            }
                        }
                        matrix[idTableRev[n1], idTableRev[n2]] = 1;
                    }
                }
            }
            return null;
        }

        // DFS based four-cycle-finder
        public static List<int> FindFourCycle3(UndirectedGraph<int, Edge<int>> graph)
        {
            foreach(var v in graph.Vertices)
            {
                var cycle = FindFourCycle3Inner(graph, new List<int>(), v, 3, v);
                if (cycle != null)
                {
                    var tmp = cycle[2];
                    cycle[2] = cycle[3];
                    cycle[3] = tmp;
                    return cycle;
                }
            }
            return null;
        }
        private static List<int> FindFourCycle3Inner(UndirectedGraph<int, Edge<int>> graph, List<int> path, int start, int depth, int goal)
        {
            path.Add(start);
            foreach(var e in graph.AdjacentEdges(start))
            {
                var n = e.GetOtherVertex(start);
                if (path.Contains(n) && depth != 0)
                    continue;
                if (depth == 0)
                {
                    if (goal == n)
                        return path;
                    else
                        return null;
                }
                var newPath = CloneList(path);
                var cycle = FindFourCycle3Inner(graph, newPath, n, depth - 1, goal);
                if(cycle != null)
                {
                    if (graph.ContainsEdge(cycle[0], cycle[2]) || graph.ContainsEdge(cycle[1], cycle[3])) //not chordless
                        continue;
                    return cycle;
                }
            }
            return null;
        }

        public static bool IsChordal2(MoplexAnalysis analysis, UndirectedGraph<int, Edge<int>> graph)
        {
            var ordering = analysis.EleminationOrder;
            var labels = analysis.NeighbourLabels;

            foreach(int v in ordering.Keys)
            {
                int o = ordering[v];
                var clique = new HashSet<int>(labels[v].Where(j => j > o).Select(j => analysis.EleminationOrderRev[j]));
                clique.Add(v);
                if(!IsClique2(clique, graph))
                {
                    return false;
                }

            }


            return true;
        }

        private static bool IsClique2(HashSet<int> clique, UndirectedGraph<int, Edge<int>> graph)
        {
            foreach (int v1 in clique)
            {
                foreach (int v2 in clique)
                {
                    if (v1 == v2)
                        continue;
                    if(!graph.ContainsEdge(v1, v2))
                    {
                        return false;
                    }
                }

            }
            return true;
        }


        // These two (ischordal + isclique) could probably be done a lot faster and better. See https://github.com/omid69/PEO-Verification
        public static bool IsChordal(MoplexAnalysis analysis)
        {
            var ordering = analysis.EleminationOrder;
            var labels = analysis.NeighbourLabels;

            foreach(int i in ordering.Keys)
            {
                int v = ordering[i];
                var clique = new HashSet<int>(labels[i].Where(j => j > v).Select(label => analysis.EleminationOrderRev[label]));
                
                clique.Add(i);
                if (!IsClique(clique, analysis.EleminationOrder,labels))
                    return false;
            }

            return true;
        }

        private static List<Edge<int>> MissingEdges(HashSet<int> clique, Dictionary<int, List<int>> labels)
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

        private static bool IsClique(HashSet<int> clique, Dictionary<int, int> order, Dictionary<int, List<int>> labels)
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
    
    public class FileDotEngine : IDotEngine
    {
        HashSet<string> added;
        public FileDotEngine(HashSet<Edge<int>> add)
        {
            added = new HashSet<string>(add.Select(e => $"{e.Source} -> {e.Target} [];"));
            foreach (var e in add)
            {
                added.Add($"{e.Target} -> {e.Source} [];");
            }
        }
        public string Run(GraphvizImageType imageType, string dot, string outputFileName)
        {
            using (var writer = new StreamWriter(outputFileName))
            {

                foreach (var s in added)
                {
                    var s1 = s.Replace("[];", "[color=\"red\"];");
                    dot = dot.Replace(s, s1);
                }
                dot = dot.Replace("->", "--");
                writer.Write(dot);
            }

            return System.IO.Path.GetFileName(outputFileName);
        }
        
    }
}
