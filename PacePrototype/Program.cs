using QuickGraph;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;

namespace PacePrototype
{
    class Program
    {
        static void Main(string[] args)
        {
            //TestGraphs.testAllGraphs();
            TestGraphs.Tests();
            //var graph1 = parse(args[0], true);
            //var graph2 = parseFile(args[0], false);
            //var graph = TestGraphs.TestGraph8();
            //var k1 = Faster.Run(graph1);
            UndirectedGraph<int, Edge<int>> graph = TestGraphs.TestGraph14();
            //if (args.Length > 0)
            //{
            //    graph = ParseFile(args[0], false);
            //}
            //else
            //{
            //    graph = ReadGraph();
            //}

            Tuple<int, HashSet<Edge<int>>> a = Faster.Run(graph);
            var k = a.Item1;
            var edgeSet = a.Item2; 
            //Idiot check
            graph.AddEdgeRange(edgeSet);
            var analysis = MoplexAnalysis.AnalyseGraph(graph, null, null);
            if (!Faster.IsChordal2(analysis, graph))
                throw new Exception("Idiot check went terribly wrong");
            //Console.WriteLine($"Graph: {args[0].Split('\\').Last()} has k={k}");
            //Console.ReadLine();
            PrintSolution(edgeSet);
        }

        private static UndirectedGraph<int, Edge<int>> ReadGraph()
        {
            var g = new UndirectedGraph<int, Edge<int>>(false);
            string line;

            while ((line = Console.ReadLine()) != null && line != "")
            {
                var edgeString = line.Split(' ');
                var edge = new[] { int.Parse(edgeString[0]), int.Parse(edgeString[1]) };
                g.AddVertexRange(edge);
                g.AddEdge(new Edge<int>(edge[0], edge[1]));
            }
            return g;
        }

        private static void PrintSolution(HashSet<Edge<int>> edgeSet)
        {
            foreach(var e in edgeSet)
            {
                Console.WriteLine($"{e.Source} {e.Target}");
            }
        }

        public static UndirectedGraph<int, Edge<int>> ParseFile(string path, bool kernelize)
        {
            var g = new UndirectedGraph<int, Edge<int>>(false);
            StreamReader sr = new StreamReader(new BufferedStream(File.OpenRead(path)));
            var verticeSet = new HashSet<int>();
            while (!sr.EndOfStream)
            {
                string line = sr.ReadLine();
                if (line == null) break;
                var edgeString = line.Split(' ');
                var edge = new[] { int.Parse(edgeString[0]), int.Parse(edgeString[1]) };
                g.AddVertexRange(edge);
                g.AddEdge(new Edge<int>(edge[0], edge[1]));
            }
            if (kernelize && File.Exists(path + "kernel"))
            {
                sr = new StreamReader(new BufferedStream(File.OpenRead(path+"kernel")));
                while (!sr.EndOfStream)
                {
                    string line = sr.ReadLine();
                    if (line == null) break;
                    var v = int.Parse(line);
                    verticeSet.Add(v);
                }
            }
            var g2 = Faster.CloneGraph(g);

            if(verticeSet.Count > 0)
            {
                foreach(int v in g.Vertices)
                {
                    if (!verticeSet.Contains(v))
                        g2.RemoveVertex(v);
                }
            }

            return g2;
        }

        

    }
}
