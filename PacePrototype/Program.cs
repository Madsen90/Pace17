using QuickGraph;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PacePrototype
{
    class Program
    {
        static void Main(string[] args)
        {
            testAllGraphs();
            Tests();
            //var graph = parse(args[0]);
            //var graph = TestGraphs.TestGraph8();
            //var k = Faster.Run(graph);
            //Console.WriteLine($"Graph: {args[0].Split('\\').Last()} has k={k}");
            //Console.ReadLine();
        }

        public static void testAllGraphs()
        {
            (var k, var edgeSet) = Faster.Run(TestGraphs.TestGraph1());
            if (edgeSet.Count != k)
                throw new Exception("edge set error on graph1");

            (k, edgeSet) = Faster.Run(TestGraphs.TestGraph2());
            if (edgeSet.Count != k)
                throw new Exception("edge set error on graph2");

            (k, edgeSet) = Faster.Run(TestGraphs.TestGraph3());
            if (edgeSet.Count != k)
                throw new Exception("edge set error on graph3");

            (k, edgeSet) = Faster.Run(TestGraphs.TestGraph4());
            if (k != 1)
                throw new Exception("k error on graph4");
            if (edgeSet.Count != k)
                throw new Exception("edge set error on graph4");

            (k, edgeSet) = Faster.Run(TestGraphs.TestGraph5());
            if (k != 2)
                throw new Exception("k error on graph5");
            if (edgeSet.Count != k)
                throw new Exception("edge set error on graph5");

            (k, edgeSet) = Faster.Run(TestGraphs.TestGraph6());
            if (k != 2)
                throw new Exception("k error on graph6");
            if (edgeSet.Count != k)
                throw new Exception("edge set error on graph6");

            (k, edgeSet) = Faster.Run(TestGraphs.TestGraph7());
            if (k != 0)
                throw new Exception("k error on graph7");
            if (edgeSet.Count != k)
                throw new Exception("edge set error on graph7");

            (k, edgeSet) = Faster.Run(TestGraphs.TestGraph8());
            if (k != 1)
                throw new Exception("k error on graph8");
            if (edgeSet.Count != k)
                throw new Exception("edge set error on graph8");

            (k, edgeSet) = Faster.Run(TestGraphs.TestGraph9());
            if (k != 0)
                throw new Exception("k error on graph9");
            if (edgeSet.Count != k)
                throw new Exception("edge set error on graph9");

            (k, edgeSet) = Faster.Run(TestGraphs.TestGraph10());
            if (k != 1)
                throw new Exception("k error on graph10");
            if (edgeSet.Count != k)
                throw new Exception("edge set error on graph10");

            (k, edgeSet) = Faster.Run(TestGraphs.TestGraph11());
            if (k != 1)
                throw new Exception("k error on graph11");
            if (edgeSet.Count != k)
                throw new Exception("edge set error on graph11");
        }

        public static UndirectedGraph<int, Edge<int>> parse(string path)
        {
            var g = new UndirectedGraph<int, Edge<int>>();
            StreamReader sr = new StreamReader(new BufferedStream(File.OpenRead(path)));
            while (!sr.EndOfStream)
            {
                string line = sr.ReadLine();
                var edge = line.Split(' ').Select(s => int.Parse(s)).ToList();
                g.AddVertexRange(edge);
                g.AddEdge(new Edge<int>(edge[0], edge[1]));

            }

            return g;
        }

        public static void Tests()
        {
            var analysis = MoplexAnalysis.AnalyseGraph(TestGraphs.TestGraph2(), null, null);
            var vStar = Faster.FindVStar(new Edge<int>(0, 7), new HashSet<int>(TestGraphs.TestGraph2().Vertices), TestGraphs.TestGraph2());
            if (vStar != 6)
                throw new Exception("Error in VStar");

            analysis = MoplexAnalysis.AnalyseGraph(TestGraphs.TestGraph8(), null, null);
            if(analysis.Moplexes.Count != 5)
                throw new Exception("Error in findmoplex without previous");

            analysis = MoplexAnalysis.AnalyseGraph(TestGraphs.TestGraph9(), new List<Edge<int>> { new Edge<int>(0, 4) }, analysis.Moplexes);
            if (analysis.Moplexes.Count != 3)
                throw new Exception("Error in findmoplex with previous");


            analysis = MoplexAnalysis.AnalyseGraph(TestGraphs.TestGraph10(), null, null);
            if (analysis.Moplexes.Count != 2)
                throw new Exception("Error in findmoplex without previous");

            analysis = MoplexAnalysis.AnalyseGraph(TestGraphs.TestGraph5(), null, null);
            if (analysis.Moplexes.Count != 4)
                throw new Exception("Error in findmoplex without previous");

            analysis = MoplexAnalysis.AnalyseGraph(TestGraphs.TestGraph11(), new List<Edge<int>> { new Edge<int>(0,4) }, analysis.Moplexes);
            if (analysis.Moplexes.Count != 3)
                throw new Exception("Error in findmoplex without previous");

            var a = Faster.FindFourCycle1(TestGraphs.TestGraph2());
            var b = Faster.FindFourCycle2(TestGraphs.TestGraph2());
            var c = Faster.FindFourCycle3(TestGraphs.TestGraph2());
            if (a != b && !a.SequenceEqual(b))
                throw new Exception("difference between findFourCycle1 & 2");
            if (a != c && !a.SequenceEqual(c))
                throw new Exception("difference between findFourCycle1 & 3");
            a = Faster.FindFourCycle1(TestGraphs.TestGraph4());
            b = Faster.FindFourCycle2(TestGraphs.TestGraph4());
            c = Faster.FindFourCycle3(TestGraphs.TestGraph4());
            if (a != b && !a.SequenceEqual(b))
                throw new Exception("difference between findFourCycle1 & 2");
            if (a != c && !a.SequenceEqual(c))
                throw new Exception("difference between findFourCycle1 & 3");
            a = Faster.FindFourCycle1(TestGraphs.TestGraph5());
            b = Faster.FindFourCycle2(TestGraphs.TestGraph5());
            c = Faster.FindFourCycle3(TestGraphs.TestGraph5());
            if (a != b && !a.SequenceEqual(b))
                throw new Exception("difference between findFourCycle1 & 2");
            if (a != c && !a.SequenceEqual(c))
                throw new Exception("difference between findFourCycle1 & 3");
            a = Faster.FindFourCycle1(TestGraphs.TestGraph6());
            b = Faster.FindFourCycle2(TestGraphs.TestGraph6());
            c = Faster.FindFourCycle3(TestGraphs.TestGraph6());
            if (a != b && !a.SequenceEqual(b))
                throw new Exception("difference between findFourCycle1 & 2");
            if (a != c && !a.SequenceEqual(c))
                throw new Exception("difference between findFourCycle1 & 3");
            a = Faster.FindFourCycle1(TestGraphs.TestGraph8());
            b = Faster.FindFourCycle2(TestGraphs.TestGraph8());
            c = Faster.FindFourCycle3(TestGraphs.TestGraph8());
            if (a != b && !a.SequenceEqual(b))
                throw new Exception("difference between findFourCycle1 & 2");
            if (a != c && !a.SequenceEqual(c))
                throw new Exception("difference between findFourCycle1 & 3");


            // Too big k test
            (var k1, var ret1) = Faster.FasterInner(TestGraphs.TestGraph8(), 8, 16, new HashSet<int>(), null, null);
            (var k2, var ret2) = Faster.Run(TestGraphs.TestGraph8());
            if ((8 - k1) != k2)
                throw new Exception("Wrong result if k is started too big");
        }

    }
}
