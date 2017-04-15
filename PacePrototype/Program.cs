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
            Tests();
            var graph = parse(args[0]);
            var k = Faster.Run(graph);
            Console.WriteLine($"Graph: {args[0].Split('\\').Last()} has k={k}");
            Console.ReadLine();
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
            var analysis = MoplexAnalysis.AnalyseGraph(TestGraphs.TestGraph2());
            var vStar = Faster.FindVStar(new Edge<int>(0, 7), new HashSet<int>(TestGraphs.TestGraph2().Vertices), TestGraphs.TestGraph2());
            var k = Faster.Run(TestGraphs.TestGraph8());



            MoplexAnalysis.AnalyseGraph(TestGraphs.TestGraph1());
            MoplexAnalysis.AnalyseGraph(TestGraphs.TestGraph2());
            MoplexAnalysis.AnalyseGraph(TestGraphs.TestGraph3());
            MoplexAnalysis.AnalyseGraph(TestGraphs.TestGraph4());
            var a = Faster.FindFourCycle1(TestGraphs.TestGraph2());
            var b = Faster.FindFourCycle2(TestGraphs.TestGraph2());
            a = Faster.FindFourCycle1(TestGraphs.TestGraph4());
            b = Faster.FindFourCycle2(TestGraphs.TestGraph4());
            a = Faster.FindFourCycle1(TestGraphs.TestGraph5());
            b = Faster.FindFourCycle2(TestGraphs.TestGraph5());
            a = Faster.FindFourCycle1(TestGraphs.TestGraph6());
            b = Faster.FindFourCycle2(TestGraphs.TestGraph6());
        }

    }
}
