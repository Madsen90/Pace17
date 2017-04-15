using QuickGraph;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PacePrototype
{
    class MoplexAnalysis
    {
        public List<List<int>> Moplexes { get; }
        public int[] EleminationOrder { get; }
        public int[] EleminationOrderRev { get; }
        public List<int>[] NeighbourLabels { get; }

        private MoplexAnalysis(List<List<int>> moplexList, int[] order, List<int>[] labels)
        {
            Moplexes = moplexList;
            EleminationOrder = order;
            EleminationOrderRev = new int[order.Length+1];
            for(int i = 0; i < order.Length; i++)
            {
                EleminationOrderRev[order[i]] = i;
            }
            NeighbourLabels = labels;
        }

        public static MoplexAnalysis AnalyseGraph(UndirectedGraph<int, Edge<int>> graph)
        {

            (int[] ordering, List<int>[] labels) = LexBFS(graph, 0);
            var moplex = FindLexMoplex(ordering, labels);

            var moplexes = FindMoplexes(graph, moplex, ordering, labels);

            return new MoplexAnalysis(moplexes, ordering, labels);
        }

        private static List<List<int>> FindMoplexes(UndirectedGraph<int, Edge<int>> graph, List<int> moplex, int[] ordering, List<int>[] labels)
        {
            var moplexes = new List<List<int>>();
            moplexes.Add(moplex); //first: add already known moplex

            var hasBeenChecked = new List<int>();

            // construct reverse lookup in ordering
            int[] revOrdering = new int[ordering.Length + 1]; 
            for (int i = 0; i < ordering.Length; i++)
            {
                revOrdering[ordering[i]] = i;
            }

            //no need to look at previously found moplex
            foreach (int v in moplex) 
            {
                hasBeenChecked.Add(v);
            }

            // Start finding new moplexes
            for (int v = 0; v < labels.Length; v++)
            {
                if (hasBeenChecked.Contains(v)) // no vertex can be part of multiple moplexes
                    continue;

                //equal neighbourhood check
                var potMoplex = new List<int>();
                for (int i = 0; i < labels.Length; i++)
                {
                    if (labels[i] != null && labels[i].SequenceEqual(labels[v]))
                        potMoplex.Add(i);
                }


                //find neighbourhood excl. the potential moplex, i.e. the seperator
                var seperator = labels[v].Select(l => revOrdering[l]).Except(potMoplex).ToList();

                
                // Check that the seperator is minimal - i.e. check all vertices in the seperator is connected at least one common component??
                // First: Remove seperator and moplex from graph (temporarily)
                var tempRemove = new List<Edge<int>>();
                foreach (int mv in potMoplex)
                {
                    tempRemove.AddRange(graph.AdjacentEdges(mv));
                    graph.RemoveVertex(mv);
                }

                foreach (var sv in seperator)
                {
                    tempRemove.AddRange(graph.AdjacentEdges(sv));
                    graph.RemoveVertex(sv);
                }

                // Find connected components in the new graph
                var a = new QuickGraph.Algorithms.ConnectedComponents.ConnectedComponentsAlgorithm<int, Edge<int>>(graph);
                a.Compute();
                var nodeToComponentDic = a.Components;

                // Add the seperator and potential moplex again
                graph.AddVertexRange(seperator);
                graph.AddVertexRange(potMoplex);
                graph.AddEdgeRange(tempRemove);


                var commonComponents = new HashSet<int>(a.Components.Values);
                foreach (var sepNode in seperator) // Find the components connected to each of the seperator vertices
                {
                    HashSet<int> connectedComponents = new HashSet<int>();
                    foreach (var e in graph.AdjacentEdges(sepNode))
                    {
                        var neighbour = e.Source == sepNode ? e.Target : e.Source;
                        if (potMoplex.Contains(neighbour))
                            continue; //not a component, since it was removed at the time
                        if (nodeToComponentDic.ContainsKey(neighbour)) //else neighbour is also neighbour of potential moplex
                        {
                            int c = -1;
                            var comp = nodeToComponentDic.TryGetValue(neighbour, out c);
                            connectedComponents.Add(c);
                        }
                    }
                    // N[sep1] intersect N[sep2] ... intersect { all components } = common components of all seperators.
                    commonComponents = new HashSet<int>(commonComponents.Intersect(connectedComponents));
                }

                // if seperators share at least one component, the seperator is minimal (intuition), and potMoplex is a moplex.
                if (commonComponents.Count > 0)
                    moplexes.Add(potMoplex);

                // To ensure no dublicates
                foreach (var n in potMoplex)
                {
                    hasBeenChecked.Add(n);
                }
            }
            return moplexes;
        }

        private static List<int> FindLexMoplex(int[] ordering, List<int>[] labels)
        {
            int i = 0;
            for (; i < ordering.Length; i++)
                if (ordering[i] == 1)
                    break;

            var moplex = new List<int>();
            for (int j = 0; j < labels.Length; j++)
                if (LexComp(labels[i], labels[j]) == 0)
                    moplex.Add(j);

            return moplex;
        }

        public static (int[], List<int>[]) LexBFS(UndirectedGraph<int, Edge<int>> graph, int start)
        {
            if (!graph.ContainsVertex(start))
                start = graph.Vertices.Min();
            List<int>[] labels = new List<int>[graph.VertexCount];
            int[] indeces = new int[graph.VertexCount];

            for (int i = 0; i < graph.VertexCount; i++)
            {
                labels[i] = new List<int>();
            }

            int next = start;
            for (int i = graph.VertexCount; i > 0; i--)
            {
                indeces[next] = i;
                labels[next].Add(i);
                Console.WriteLine($"next: {next}");
                foreach (Edge<int> outEdge in graph.AdjacentEdges(next))
                {
                    var neighbour = outEdge.Source == next ? outEdge.Target : outEdge.Source;
                    labels[neighbour].Add(i);
                }

                int max = -1;
                for (int j = 0; j < labels.Length; j++)
                {
                    if (indeces[j] == 0 && (max == -1 || LexComp(labels[j], labels[max]) > 0))
                    {
                        max = j;
                    }
                }
                next = max;
            }
            return (indeces, labels);
        }

        private static int LexComp(List<int> l1, List<int> l2)
        {
            for (int i = 0; i < Math.Min(l1.Count, l2.Count); i++)
            {
                if (l1[i] < l2[i])
                    return -1;

                if (l1[i] > l2[i])
                    return 1;
            }

            return l1.Count - l2.Count;
        }
    }
}
