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
            int k = -1;
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
                return Math.Max(FasterInner(graph1, k - 1, r1, Marked1), FasterInner(graph1, k - 1, r2, Marked2));
            }
            

            return -1;
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
        private static bool IsChordal(MoplexAnalysis analysis)
        {
            var ordering = analysis.EleminationOrder;
            var labels = analysis.NeighbourLabels;

            for (int i = ordering.Length -1; i >= 0; i--)
            {
                int v = ordering[i];
                var clique = new HashSet<int>(labels[i].Where(j => j < i));
                clique.Add(i);
                if (!IsClique(clique, labels))
                    return false;
            }

            return true;
        }

        private static bool IsClique(HashSet<int> clique, List<int>[] labels)
        {
            foreach (var vertex in clique)
            {
                foreach(var neighbour in clique)
                {
                    if (vertex == neighbour)
                        continue;
                    if (!labels[neighbour].Contains(vertex))
                        return false;
                }
            }
            return true;
        }
    }

}
