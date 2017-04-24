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
        public Dictionary<int, int> EleminationOrder { get; }
        public Dictionary<int, int> EleminationOrderRev { get; }
        public Dictionary<int, List<int>> NeighbourLabels { get; }
        public static int rounds = 0;

        private MoplexAnalysis(List<List<int>> moplexList, Dictionary<int, int> revOrder, Dictionary<int, int> order, Dictionary<int, List<int>> labels)
        {
            Moplexes = moplexList;
            EleminationOrder = order;
            EleminationOrderRev = revOrder;
            NeighbourLabels = labels;
        }

        public static MoplexAnalysis AnalyseGraph(UndirectedGraph<int, Edge<int>> graph)
        {
            rounds++;
            
            (Dictionary<int, int> ordering, Dictionary<int, List<int>> labels) = LexBFS(graph, 0);
            var revOrder = new Dictionary<int, int>();
            foreach (int i in ordering.Keys)
            {
                revOrder[ordering[i]] = i;
            }

            var moplexes = FindMoplexes(graph, revOrder, ordering, labels);

            return new MoplexAnalysis(moplexes, revOrder, ordering, labels);
        }

        private static List<List<int>> FindMoplexes(UndirectedGraph<int, Edge<int>> graph, Dictionary<int, int> revOrdering, Dictionary<int, int> ordering, Dictionary<int, List<int>> labels)
        {
            var moplexes = new List<List<int>>();

            var hasBeenChecked = new List<int>();
            

            // Start finding new moplexes
            foreach(var v in labels.Keys)
            {
                if (hasBeenChecked.Contains(v)) // no vertex can be part of multiple moplexes
                    continue;

                //equal neighbourhood check
                var potMoplex = new List<int>();
                foreach (var i in labels.Keys)
                {
                    if (labels[i] != null && labels[i].SequenceEqual(labels[v]))
                        potMoplex.Add(i);
                }


                //find neighbourhood excl. the potential moplex, i.e. the seperator
                var seperator = labels[v].Select(l => revOrdering[l]).Except(potMoplex).ToList();

                
                // Check that the seperator is minimal - i.e. check all vertices in the seperator is connected to all components
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

                var isMoplex = true;
                foreach (var sepNode in seperator) // Find the components connected to each of the seperator vertices
                {
                    HashSet<int> connectedComponents = new HashSet<int>();
                    foreach (var e in graph.AdjacentEdges(sepNode))
                    {
                        var neighbour = e.GetOtherVertex(sepNode);
                        if (potMoplex.Contains(neighbour))
                            continue; //not a component, since it was removed at the time
                        if (nodeToComponentDic.ContainsKey(neighbour)) //else neighbour is also seperator TODO: error here, 
                        {
                            int c = -1;
                            nodeToComponentDic.TryGetValue(neighbour, out c);
                            connectedComponents.Add(c);
                        }
                    }
                    if (connectedComponents.Count < a.ComponentCount)
                    {
                        isMoplex = false;
                        break;
                    }
                }

                if (isMoplex)
                {
                    moplexes.Add(potMoplex);
                }
                

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

        public static (int[], List<int>[]) LexBFS2(UndirectedGraph<int, Edge<int>> graph, int start)
        {
            if (!graph.ContainsVertex(start))
                start = graph.Vertices.Min();
            List<int>[] labels = new List<int>[graph.VertexCount];
            //Console.WriteLine($"New lables list of size {labels.Length}");
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
                //Console.WriteLine($"next: {next}");
                foreach (Edge<int> outEdge in graph.AdjacentEdges(next))
                {
                    var neighbour = outEdge.GetOtherVertex(next);
                    //Console.WriteLine($"Neighbour: {neighbour}, Labels.length: {labels.Length}, rounds: {rounds}, graph.containsVertex(neighbour): {graph.ContainsVertex(neighbour)}");
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

        public static (Dictionary<int, int>, Dictionary<int, List<int>>) LexBFS(UndirectedGraph<int, Edge<int>> graph, int start)
        {
            if (!graph.ContainsVertex(start))
                start = graph.Vertices.Min();
            var labels = new Dictionary<int, List<int>>(); 
            var indeces = new Dictionary<int, int>();

            foreach (var v in graph.Vertices)
            {
                labels[v] = new List<int>();
                indeces[v] = -1;
            }

            int next = start;

            for (int i = graph.VertexCount; i > 0; i--)
            {
                indeces[next] = i;
                labels[next].Add(i);
                //Console.WriteLine($"next: {next}");
                foreach (Edge<int> outEdge in graph.AdjacentEdges(next))
                {
                    var neighbour = outEdge.GetOtherVertex(next);
                    //Console.WriteLine($"Neighbour: {neighbour}, Labels.length: {labels.Length}, rounds: {rounds}, graph.containsVertex(neighbour): {graph.ContainsVertex(neighbour)}");
                    labels[neighbour].Add(i);
                }

                int max = -1;
                foreach(int j in labels.Keys)
                {
                    if (indeces[j] == -1 && (max == -1 || LexComp(labels[j], labels[max]) > 0))
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
