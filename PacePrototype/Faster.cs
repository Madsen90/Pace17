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
        public static MoplexAnalysis AnalyseGraph(UndirectedGraph<int, Edge<int>> graph)
        {
            
            (int[] ordering, List<int>[] labels) = LexBFS(graph, 0);
            var moplex = FindLexMoplex(ordering, labels);

            var moplexes = FindMoplexes(graph, moplex, ordering, labels);

            return new MoplexAnalysis();
        }

        private static List<List<int>> FindMoplexes(UndirectedGraph<int, Edge<int>> graph, List<int> moplex, int[] ordering, List<int>[] labels)
        {
            var moplexes = new List<List<int>>();
            moplexes.Add(moplex);

            var hasBeenChecked = new List<int>();
            int[] revOrdering = new int[ordering.Length+1];
            for(int i = 0; i < ordering.Length; i++)
            {
                revOrdering[ordering[i]] = i;
            }

            foreach (int v in moplex) //no need to look at previouslt found moplex
            {
                hasBeenChecked.Add(v);
            }

            for(int v = 0; v < labels.Length; v++)
            {
                if (hasBeenChecked.Contains(v)) //already been checked
                    continue;

                //equal neighbourhood check
                var potMoplex = new List<int>();
                for(int i = 0; i < labels.Length; i++)
                {
                    if (labels[i] != null && labels[i].SequenceEqual(labels[v]))
                        potMoplex.Add(i);
                }
                
                
                //find exclusive neighbourhood
                var seperator = labels[v].Select(l => revOrdering[l]).Except(potMoplex).ToList();

                //minimal seperator neighbourhood check

                var tempRemove= new List<Edge<int>>();
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
                //graph.RemoveEdges(tempRemove);

                var a = new QuickGraph.Algorithms.ConnectedComponents.ConnectedComponentsAlgorithm<int, Edge<int>>(graph);
                a.Compute();
                var nodeToComponentDic = a.Components;

                graph.AddVertexRange(seperator);
                graph.AddVertexRange(potMoplex);
                graph.AddEdgeRange(tempRemove);
                var commonComponents = new HashSet<int>(a.Components.Values);
                foreach(var sepNode in seperator)
                {
                    HashSet<int> connectedComponents = new HashSet<int>();
                    foreach(var e in graph.AdjacentEdges(sepNode))
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
                    commonComponents = new HashSet<int>(commonComponents.Intersect(connectedComponents));
                }
                
                if (commonComponents.Count > 0)
                    moplexes.Add(potMoplex);
                foreach(var n in potMoplex)
                {
                    hasBeenChecked.Add(n);//to ensure no dublicates
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
            for(int j = 0; j < labels.Length; j++)
                if(LexComp(labels[i], labels[j]) == 0)
                    moplex.Add(j);

            return moplex;
        }

        private static (int[], List<int>[]) LexBFS(UndirectedGraph<int, Edge<int>> graph, int start)
        {
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
                Console.WriteLine($"v:{next} has {graph.AdjacentEdges(next).Count()} neighbours");
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
            for(int i = 0; i < Math.Min(l1.Count, l2.Count); i++)
            {
                if (l1[i] < l2[i])
                    return -1;

                if (l1[i] > l2[i])
                    return 1;
            }

            return l1.Count - l2.Count;
        }

        //Graph from Separability Generalizes Dirac's Theorem
        public static UndirectedGraph<int, Edge<int>> TestGraph1()
        {
            var g = new UndirectedGraph<int, Edge<int>>();
            g.AddVertexRange(new int[] { 0, 1, 2, 3, 4, 5, 6, 7 });
            var edges = new List<Edge<int>>
            {
                new Edge<int>(0, 1),
                new Edge<int>(0, 2),
                new Edge<int>(0, 4),
                new Edge<int>(0, 5),
                new Edge<int>(1, 2),
                new Edge<int>(1, 4),
                new Edge<int>(1, 5),
                new Edge<int>(2, 5),
                new Edge<int>(2, 6),
                new Edge<int>(3, 5),
                new Edge<int>(3, 6),
                new Edge<int>(4, 7),
                new Edge<int>(5, 7),
                new Edge<int>(6, 7)
            };
            g.AddEdgeRange(edges);
            
            return g;
        }

        // Graph with 3-vertex moplex {0,1,2}, 6-vertex cycle, and "false" moplex triangle
        public static UndirectedGraph<int, Edge<int>> TestGraph2()
        {
            var g = new UndirectedGraph<int, Edge<int>>();
            g.AddVertexRange(new int[] { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 });
            var edges = new List<Edge<int>>
            {
                new Edge<int>(0, 1),
                new Edge<int>(0, 2),
                new Edge<int>(0, 3),
                new Edge<int>(1, 2),
                new Edge<int>(1, 3),
                new Edge<int>(2, 3),
                new Edge<int>(3, 4),
                new Edge<int>(3, 10),
                new Edge<int>(4, 5),
                new Edge<int>(4, 6),
                new Edge<int>(5, 6),
                new Edge<int>(5, 8),
                new Edge<int>(6, 7),
                new Edge<int>(8, 9),
                new Edge<int>(9, 10)
            };
            g.AddEdgeRange(edges);

            return g;
        }

        // Graph where all vertices are moplexes
        public static UndirectedGraph<int, Edge<int>> TestGraph3()
        {
            var g = new UndirectedGraph<int, Edge<int>>();
            g.AddVertexRange(new int[] { 0, 1, 2, 3, 4, 5});
            var edges = new List<Edge<int>>
            {
                new Edge<int>(0, 3),
                new Edge<int>(0, 4),
                new Edge<int>(0, 5),
                new Edge<int>(1, 3),
                new Edge<int>(1, 4),
                new Edge<int>(1, 5),
                new Edge<int>(2, 3),
                new Edge<int>(2, 4),
                new Edge<int>(2, 5)

            };
            g.AddEdgeRange(edges);

            return g;
        }

    }

    internal class MoplexAnalysis
    {
    }

}
