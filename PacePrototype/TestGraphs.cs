using QuickGraph;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PacePrototype
{
    public class TestGraphs
    {
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
            g.AddVertexRange(new int[] { 0, 1, 2, 3, 4, 5 });
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

        // one four cycle, k = 1
        public static UndirectedGraph<int, Edge<int>> TestGraph4()
        {
            var g = new UndirectedGraph<int, Edge<int>>();
            g.AddVertexRange(new int[] { 0, 1, 2, 3 });
            var edges = new List<Edge<int>>
            {
                new Edge<int>(0, 1),
                new Edge<int>(0, 2),
                new Edge<int>(1, 3),
                new Edge<int>(2, 3),


            };
            g.AddEdgeRange(edges);

            return g;
        }

        // two four cycles, k = 2
        public static UndirectedGraph<int, Edge<int>> TestGraph5()
        {
            var g = new UndirectedGraph<int, Edge<int>>();
            g.AddVertexRange(new int[] { 0, 1, 2, 3, 4, 5 });
            var edges = new List<Edge<int>>
            {
                new Edge<int>(0, 1),
                new Edge<int>(0, 2),
                new Edge<int>(1, 3),
                new Edge<int>(2, 3),
                new Edge<int>(2, 5),
                new Edge<int>(3, 4),
                new Edge<int>(4, 5),

            };
            g.AddEdgeRange(edges);

            return g;
        }

        // two four cycles and one five cycle, k = 2
        public static UndirectedGraph<int, Edge<int>> TestGraph6()
        {
            var g = new UndirectedGraph<int, Edge<int>>();
            g.AddVertexRange(new int[] {  0, 1, 2, 3, 4, 5 });
            var edges = new List<Edge<int>>
            {
                new Edge<int>(0, 1),
                new Edge<int>(0, 5),
                new Edge<int>(0, 2),
                new Edge<int>(1, 3),
                new Edge<int>(2, 3),
                new Edge<int>(3, 4),
                new Edge<int>(4, 5),


            };
            g.AddEdgeRange(edges);

            return g;
        }

        //chordal graph, k = 0
        public static UndirectedGraph<int, Edge<int>> TestGraph7()
        {
            var g = new UndirectedGraph<int, Edge<int>>();
            g.AddVertexRange(new int[] { 0, 1, 2});
            var edges = new List<Edge<int>>
            {
                new Edge<int>(0, 1),
                new Edge<int>(0, 2),
                new Edge<int>(1, 2),


            };
            g.AddEdgeRange(edges);

            return g;
        }

        // letter graph missing two diagonal edges, k = 1
        public static UndirectedGraph<int, Edge<int>> TestGraph8()
        {
            var g = new UndirectedGraph<int, Edge<int>>();
            g.AddVertexRange(new int[] { 0, 1, 2, 3, 4 });
            var edges = new List<Edge<int>>
            {
                new Edge<int>(0, 1),
                new Edge<int>(0, 2),
                new Edge<int>(0, 3),
                new Edge<int>(1, 4),
                new Edge<int>(2, 4),
                new Edge<int>(3, 4),


            };
            g.AddEdgeRange(edges);

            return g;
        }

        //reordered testgraph 8 to test for weird cycle finder behavior
        public static UndirectedGraph<int, Edge<int>> TestGraph8reorder()
        {
            var g = new UndirectedGraph<int, Edge<int>>();
            g.AddVertexRange(new int[] { 4, 1, 2, 3, 0 });
            var edges = new List<Edge<int>>
            {
                new Edge<int>(3, 4),
                new Edge<int>(2, 4),
                new Edge<int>(1, 4),
                new Edge<int>(0, 1),
                new Edge<int>(0, 2),
                new Edge<int>(0, 3),


            };
            g.AddEdgeRange(edges);

            return g;
        }

        // Graph 8 as chordal, k = 0
        public static UndirectedGraph<int, Edge<int>> TestGraph9()
        {
            var g = new UndirectedGraph<int, Edge<int>>();
            g.AddVertexRange(new int[] { 0, 1, 2, 3, 4 });
            var edges = new List<Edge<int>>
            {
                new Edge<int>(0, 1),
                new Edge<int>(0, 2),
                new Edge<int>(0, 3),
                new Edge<int>(0, 4),
                new Edge<int>(1, 4),
                new Edge<int>(2, 4),
                new Edge<int>(3, 4),


            };
            g.AddEdgeRange(edges);

            return g;
        }

        // Testgraph 5 with missing edge between 2 and 5
        public static UndirectedGraph<int, Edge<int>> TestGraph10()
        {
            var g = new UndirectedGraph<int, Edge<int>>();
            g.AddVertexRange(new int[] { 0, 1, 2, 3, 4, 5 });
            var edges = new List<Edge<int>>
            {
                new Edge<int>(0, 1),
                new Edge<int>(0, 2),
                new Edge<int>(1, 3),
                new Edge<int>(2, 3),
                new Edge<int>(3, 4),
                new Edge<int>(4, 5),

            };
            g.AddEdgeRange(edges);

            return g;
        }

        // Testgraph 5 with extra edge between 2 and 4
        public static UndirectedGraph<int, Edge<int>> TestGraph11()
        {
            var g = new UndirectedGraph<int, Edge<int>>();
            g.AddVertexRange(new int[] { 0, 1, 2, 3, 4, 5 });
            var edges = new List<Edge<int>>
            {
                new Edge<int>(0, 1),
                new Edge<int>(0, 2),
                new Edge<int>(1, 3),
                new Edge<int>(2, 3),
                new Edge<int>(2, 4),
                new Edge<int>(2, 5),
                new Edge<int>(3, 4),
                new Edge<int>(4, 5),

            };
            g.AddEdgeRange(edges);

            return g;
        }

        // two disjoint four cycles
        public static UndirectedGraph<int, Edge<int>> TestGraph12()
        {
            var g = new UndirectedGraph<int, Edge<int>>();
            g.AddVertexRange(new int[] { 0, 1, 2, 3, 4, 5, 6, 7 });
            var edges = new List<Edge<int>>
            {
                new Edge<int>(0, 1),
                new Edge<int>(0, 2),
                new Edge<int>(1, 3),
                new Edge<int>(2, 3),

                new Edge<int>(4, 5),
                new Edge<int>(4, 6),
                new Edge<int>(5, 7),
                new Edge<int>(6, 7)
            };
            g.AddEdgeRange(edges);

            return g;
        }

        // troublesome component from instances/5.graph
        public static UndirectedGraph<int, Edge<int>> TestGraph13()
        {
            var g = new UndirectedGraph<int, Edge<int>>();
            var edges = new List<Edge<int>>
            {
                new Edge<int>(29, 9),
                new Edge<int>(39, 29),
                new Edge<int>(9, 85),
                new Edge<int>(39, 20),
                new Edge<int>(39, 91),
                new Edge<int>(39, 4),
                new Edge<int>(47, 39),
                new Edge<int>(39, 97),
                new Edge<int>(37, 85),
                new Edge<int>(95, 85),
                new Edge<int>(26, 59),
                new Edge<int>(53, 26),
                new Edge<int>(24, 53),
                new Edge<int>(67, 20),
                new Edge<int>(72, 67),
                new Edge<int>(81, 20),
                new Edge<int>(68, 72),
                new Edge<int>(81, 30),
                new Edge<int>(74, 81),
                new Edge<int>(48, 75),
                new Edge<int>(18, 48),
                new Edge<int>(48, 60),
                new Edge<int>(75, 62),
                new Edge<int>(18, 70),
                new Edge<int>(18, 82),
                new Edge<int>(18, 66),
                new Edge<int>(18, 60),
                new Edge<int>(34, 18),
                new Edge<int>(18, 49),
                new Edge<int>(66, 62),
                new Edge<int>(73, 62),
                new Edge<int>(24, 50),
                new Edge<int>(24, 5),
                new Edge<int>(24, 47),
                new Edge<int>(83, 24),
                new Edge<int>(50, 86),
                new Edge<int>(13, 83),
                new Edge<int>(86, 56),
                new Edge<int>(25, 66),
                new Edge<int>(34, 49),
                new Edge<int>(10, 34),
                new Edge<int>(25, 49),
                new Edge<int>(65, 93),
                new Edge<int>(94, 65),
                new Edge<int>(65, 3),
                new Edge<int>(89, 65),
                new Edge<int>(65, 68),
                new Edge<int>(46, 93),
                new Edge<int>(89, 94),
                new Edge<int>(3, 22),
                new Edge<int>(89, 25),
                new Edge<int>(89, 73),
                new Edge<int>(46, 80),
                new Edge<int>(77, 46),
                new Edge<int>(46, 52),
                new Edge<int>(17, 0),
                new Edge<int>(92, 17),
                new Edge<int>(17, 45),
                new Edge<int>(0, 15),
                new Edge<int>(45, 12),
                new Edge<int>(45, 8),
                new Edge<int>(45, 19),
                new Edge<int>(45, 32),
                new Edge<int>(15, 54),
                new Edge<int>(91, 1),
                new Edge<int>(4, 91),
                new Edge<int>(4, 43),
                new Edge<int>(12, 37),
                new Edge<int>(25, 10),
                new Edge<int>(73, 22),
                new Edge<int>(30, 95),
                new Edge<int>(74, 55),
                new Edge<int>(56, 21),
                new Edge<int>(56, 42),
                new Edge<int>(21, 27),
                new Edge<int>(42, 99)
            };
            foreach (var edge in edges)
            {
                if (!g.ContainsVertex(edge.Source))
                    g.AddVertex(edge.Source);
                if (!g.ContainsVertex(edge.Target))
                    g.AddVertex(edge.Target);
            }
            g.AddEdgeRange(edges);

            return g;
        }

        // troublesome component from instances/5.graph without two single-edge-connected-components which should have no minfil edges 
        public static UndirectedGraph<int, Edge<int>> TestGraph14()
        {
            var g = new UndirectedGraph<int, Edge<int>>();
            var edges = new List<Edge<int>>
            {
                new Edge<int>(29, 9),
                new Edge<int>(39, 29),
                new Edge<int>(9, 85),
                new Edge<int>(39, 20),
                new Edge<int>(39, 91),
                new Edge<int>(39, 4),
                new Edge<int>(47, 39),
                new Edge<int>(39, 97),
                new Edge<int>(95, 85),
                new Edge<int>(67, 20),
                new Edge<int>(72, 67),
                new Edge<int>(81, 20),
                new Edge<int>(68, 72),
                new Edge<int>(81, 30),
                new Edge<int>(74, 81),
                new Edge<int>(48, 75),
                new Edge<int>(18, 48),
                new Edge<int>(48, 60),
                new Edge<int>(75, 62),
                new Edge<int>(18, 70),
                new Edge<int>(18, 82),
                new Edge<int>(18, 66),
                new Edge<int>(18, 60),
                new Edge<int>(34, 18),
                new Edge<int>(18, 49),
                new Edge<int>(66, 62),
                new Edge<int>(73, 62),
                new Edge<int>(25, 66),
                new Edge<int>(34, 49),
                new Edge<int>(10, 34),
                new Edge<int>(25, 49),
                new Edge<int>(65, 93),
                new Edge<int>(94, 65),
                new Edge<int>(65, 3),
                new Edge<int>(89, 65),
                new Edge<int>(65, 68),
                new Edge<int>(46, 93),
                new Edge<int>(89, 94),
                new Edge<int>(3, 22),
                new Edge<int>(89, 25),
                new Edge<int>(89, 73),
                new Edge<int>(46, 80),
                new Edge<int>(77, 46),
                new Edge<int>(46, 52),
                new Edge<int>(91, 1),
                new Edge<int>(4, 91),
                new Edge<int>(4, 43),
                new Edge<int>(25, 10),
                new Edge<int>(73, 22),
                new Edge<int>(30, 95),
                new Edge<int>(74, 55),
            };
            foreach (var edge in edges)
            {
                if (!g.ContainsVertex(edge.Source))
                    g.AddVertex(edge.Source);
                if (!g.ContainsVertex(edge.Target))
                    g.AddVertex(edge.Target);
            }
            g.AddEdgeRange(edges);

            return g;
        }

        // weird subset of testgraph14 with added edges, where findcycle2 finds cycle and 3 does not
        public static UndirectedGraph<int, Edge<int>> TestGraph15()
        {
            var g = new UndirectedGraph<int, Edge<int>>();
            g.AddVertexRange(new int[] { 22, 39, 95, 48, 73, 29, 65, 3, 68, 25, 30, 89, 85, 67, 18, 94, 72, 66, 62, 81, 20 });
            var edges = new List<Edge<int>>
            {
                new Edge<int>(3, 22),
                new Edge<int>(73, 22),
                new Edge<int>(39, 29),
                new Edge<int>(39, 20),
                new Edge<int>(95, 85),
                new Edge<int>(30, 95),
                new Edge<int>(48, 66),
                new Edge<int>(48, 62),
                new Edge<int>(18, 48),
                new Edge<int>(3, 73),
                new Edge<int>(73, 62),
                new Edge<int>(89, 73),
                new Edge<int>(29, 85),
                new Edge<int>(94, 65),
                new Edge<int>(89, 65),
                new Edge<int>(65, 68),
                new Edge<int>(65, 3),
                new Edge<int>(68, 72),
                new Edge<int>(25, 66),
                new Edge<int>(18, 25),
                new Edge<int>(89, 25),
                new Edge<int>(81, 30),
                new Edge<int>(89, 94),
                new Edge<int>(72, 67),
                new Edge<int>(67, 20),
                new Edge<int>(18, 66),
                new Edge<int>(66, 62),
                new Edge<int>(81, 20)
            };
            g.AddEdgeRange(edges);


            return g;
        }


        public static void testAllGraphs()
        {
            (var k, var edgeSet) = Faster.Run(TestGraphs.TestGraph1());
            if (edgeSet.Count != k)
                throw new Exception("edge set error on graph1");

            (k, edgeSet) = Faster.Run(TestGraphs.TestGraph2());
            if (k != 3)
                throw new Exception("k error on graph2");
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

            (k, edgeSet) = Faster.Run(TestGraphs.TestGraph12());
            if (k != 2)
                throw new Exception("k error on graph11");
            if (edgeSet.Count != k)
                throw new Exception("edge set error on graph11");
        }

        public static void Tests()
        {
        //    //Kernel tests
        //    //TestGraph 1
        //    var noKernel = Faster.Run(TestGraph1());
        //    var withKernel = Faster.RunWithKernel(TestGraph1());
        //    if (noKernel.Item1 != withKernel.Item1)
        //        throw new Exception("different k in kernel test TestGraph1");
        //    var noKernelTestGraph = TestGraph1();
        //    var withKernelTestGraph = TestGraph1();
        //    noKernelTestGraph.AddEdgeRange(noKernel.Item2);
        //    withKernelTestGraph.AddEdgeRange(withKernel.Item2);
        //    var noKernelAnalysis = MoplexAnalysis.AnalyseGraph(noKernelTestGraph, null, null);
        //    var withKernelAnalysis = MoplexAnalysis.AnalyseGraph(withKernelTestGraph, null, null);
        //    if (!Faster.IsChordal2(noKernelAnalysis, noKernelTestGraph))
        //        throw new Exception("nokernel is not chordal TestGraph1");
        //    if (!Faster.IsChordal2(withKernelAnalysis, withKernelTestGraph))
        //        throw new Exception("withkernel is not chordal TestGraph1");

        //    //TestGraph2
        //    noKernel = Faster.Run(TestGraph2());
        //    withKernel = Faster.RunWithKernel(TestGraph2());
        //    if (noKernel.Item1 != withKernel.Item1)
        //        throw new Exception("different k in kernel test TestGraph2");
        //    noKernelTestGraph = TestGraph2();
        //    withKernelTestGraph = TestGraph2();
        //    noKernelTestGraph.AddEdgeRange(noKernel.Item2);
        //    withKernelTestGraph.AddEdgeRange(withKernel.Item2);
        //    noKernelAnalysis = MoplexAnalysis.AnalyseGraph(noKernelTestGraph, null, null);
        //    withKernelAnalysis = MoplexAnalysis.AnalyseGraph(withKernelTestGraph, null, null);
        //    if (!Faster.IsChordal2(noKernelAnalysis, noKernelTestGraph))
        //        throw new Exception("nokernel is not chordal TestGraph2");
        //    if (!Faster.IsChordal2(withKernelAnalysis, withKernelTestGraph))
        //        throw new Exception("withkernel is not chordal TestGraph2");

        //    //TestGraph3
        //    noKernel = Faster.Run(TestGraph3());
        //    withKernel = Faster.RunWithKernel(TestGraph3());
        //    if (noKernel.Item1 != withKernel.Item1)
        //        throw new Exception("different k in kernel test TestGraph3");
        //    noKernelTestGraph = TestGraph3();
        //    withKernelTestGraph = TestGraph3();
        //    noKernelTestGraph.AddEdgeRange(noKernel.Item2);
        //    withKernelTestGraph.AddEdgeRange(withKernel.Item2);
        //    noKernelAnalysis = MoplexAnalysis.AnalyseGraph(noKernelTestGraph, null, null);
        //    withKernelAnalysis = MoplexAnalysis.AnalyseGraph(withKernelTestGraph, null, null);
        //    if (!Faster.IsChordal2(noKernelAnalysis, noKernelTestGraph))
        //        throw new Exception("nokernel is not chordal TestGraph3");
        //    if (!Faster.IsChordal2(withKernelAnalysis, withKernelTestGraph))
        //        throw new Exception("withkernel is not chordal TestGraph3");

        //    //TestGraph4
        //    noKernel = Faster.Run(TestGraph4());
        //    withKernel = Faster.RunWithKernel(TestGraph4());
        //    if (noKernel.Item1 != withKernel.Item1)
        //        throw new Exception("different k in kernel test TestGraph4");
        //    noKernelTestGraph = TestGraph4();
        //    withKernelTestGraph = TestGraph4();
        //    noKernelTestGraph.AddEdgeRange(noKernel.Item2);
        //    withKernelTestGraph.AddEdgeRange(withKernel.Item2);
        //    noKernelAnalysis = MoplexAnalysis.AnalyseGraph(noKernelTestGraph, null, null);
        //    withKernelAnalysis = MoplexAnalysis.AnalyseGraph(withKernelTestGraph, null, null);
        //    if (!Faster.IsChordal2(noKernelAnalysis, noKernelTestGraph))
        //        throw new Exception("nokernel is not chordal TestGraph4");
        //    if (!Faster.IsChordal2(withKernelAnalysis, withKernelTestGraph))
        //        throw new Exception("withkernel is not chordal TestGraph4");

        //    //TestGraph5
        //    noKernel = Faster.Run(TestGraph5());
        //    withKernel = Faster.RunWithKernel(TestGraph5());
        //    if (noKernel.Item1 != withKernel.Item1)
        //        throw new Exception("different k in kernel test TestGraph5");
        //    noKernelTestGraph = TestGraph5();
        //    withKernelTestGraph = TestGraph5();
        //    noKernelTestGraph.AddEdgeRange(noKernel.Item2);
        //    withKernelTestGraph.AddEdgeRange(withKernel.Item2);
        //    noKernelAnalysis = MoplexAnalysis.AnalyseGraph(noKernelTestGraph, null, null);
        //    withKernelAnalysis = MoplexAnalysis.AnalyseGraph(withKernelTestGraph, null, null);
        //    if (!Faster.IsChordal2(noKernelAnalysis, noKernelTestGraph))
        //        throw new Exception("nokernel is not chordal TestGraph5");
        //    if (!Faster.IsChordal2(withKernelAnalysis, withKernelTestGraph))
        //        throw new Exception("withkernel is not chordal TestGraph5");

        //    //TestGraph6
        //    noKernel = Faster.Run(TestGraph6());
        //    withKernel = Faster.RunWithKernel(TestGraph6());
        //    if (noKernel.Item1 != withKernel.Item1)
        //        throw new Exception("different k in kernel test TestGraph6");
        //    noKernelTestGraph = TestGraph6();
        //    withKernelTestGraph = TestGraph6();
        //    noKernelTestGraph.AddEdgeRange(noKernel.Item2);
        //    withKernelTestGraph.AddEdgeRange(withKernel.Item2);
        //    noKernelAnalysis = MoplexAnalysis.AnalyseGraph(noKernelTestGraph, null, null);
        //    withKernelAnalysis = MoplexAnalysis.AnalyseGraph(withKernelTestGraph, null, null);
        //    if (!Faster.IsChordal2(noKernelAnalysis, noKernelTestGraph))
        //        throw new Exception("nokernel is not chordal TestGraph6");
        //    if (!Faster.IsChordal2(withKernelAnalysis, withKernelTestGraph))
        //        throw new Exception("withkernel is not chordal TestGraph6");


        //    //TestGraph7
        //    noKernel = Faster.Run(TestGraph7());
        //    withKernel = Faster.RunWithKernel(TestGraph7());
        //    if (noKernel.Item1 != withKernel.Item1)
        //        throw new Exception("different k in kernel test TestGraph7");
        //    noKernelTestGraph = TestGraph7();
        //    withKernelTestGraph = TestGraph7();
        //    noKernelTestGraph.AddEdgeRange(noKernel.Item2);
        //    withKernelTestGraph.AddEdgeRange(withKernel.Item2);
        //    noKernelAnalysis = MoplexAnalysis.AnalyseGraph(noKernelTestGraph, null, null);
        //    withKernelAnalysis = MoplexAnalysis.AnalyseGraph(withKernelTestGraph, null, null);
        //    if (!Faster.IsChordal2(noKernelAnalysis, noKernelTestGraph))
        //        throw new Exception("nokernel is not chordal TestGraph7");
        //    if (!Faster.IsChordal2(withKernelAnalysis, withKernelTestGraph))
        //        throw new Exception("withkernel is not chordal TestGraph7");


        //    //TestGraph8
        //    noKernel = Faster.Run(TestGraph8());
        //    withKernel = Faster.RunWithKernel(TestGraph8());
        //    if (noKernel.Item1 != withKernel.Item1)
        //        throw new Exception("different k in kernel test TestGraph8");
        //    noKernelTestGraph = TestGraph8();
        //    withKernelTestGraph = TestGraph8();
        //    noKernelTestGraph.AddEdgeRange(noKernel.Item2);
        //    withKernelTestGraph.AddEdgeRange(withKernel.Item2);
        //    noKernelAnalysis = MoplexAnalysis.AnalyseGraph(noKernelTestGraph, null, null);
        //    withKernelAnalysis = MoplexAnalysis.AnalyseGraph(withKernelTestGraph, null, null);
        //    if (!Faster.IsChordal2(noKernelAnalysis, noKernelTestGraph))
        //        throw new Exception("nokernel is not chordal TestGraph8");
        //    if (!Faster.IsChordal2(withKernelAnalysis, withKernelTestGraph))
        //        throw new Exception("withkernel is not chordal TestGraph8");


        //    //TestGraph9
        //    noKernel = Faster.Run(TestGraph9());
        //    withKernel = Faster.RunWithKernel(TestGraph9());
        //    if (noKernel.Item1 != withKernel.Item1)
        //        throw new Exception("different k in kernel test TestGraph9");
        //    noKernelTestGraph = TestGraph9();
        //    withKernelTestGraph = TestGraph9();
        //    noKernelTestGraph.AddEdgeRange(noKernel.Item2);
        //    withKernelTestGraph.AddEdgeRange(withKernel.Item2);
        //    noKernelAnalysis = MoplexAnalysis.AnalyseGraph(noKernelTestGraph, null, null);
        //    withKernelAnalysis = MoplexAnalysis.AnalyseGraph(withKernelTestGraph, null, null);
        //    if (!Faster.IsChordal2(noKernelAnalysis, noKernelTestGraph))
        //        throw new Exception("nokernel is not chordal TestGraph9");
        //    if (!Faster.IsChordal2(withKernelAnalysis, withKernelTestGraph))
        //        throw new Exception("withkernel is not chordal TestGraph9");


        //    //TestGraph10
        //    noKernel = Faster.Run(TestGraph10());
        //    withKernel = Faster.RunWithKernel(TestGraph10());
        //    if (noKernel.Item1 != withKernel.Item1)
        //        throw new Exception("different k in kernel test TestGraph10");
        //    noKernelTestGraph = TestGraph10();
        //    withKernelTestGraph = TestGraph10();
        //    noKernelTestGraph.AddEdgeRange(noKernel.Item2);
        //    withKernelTestGraph.AddEdgeRange(withKernel.Item2);
        //    noKernelAnalysis = MoplexAnalysis.AnalyseGraph(noKernelTestGraph, null, null);
        //    withKernelAnalysis = MoplexAnalysis.AnalyseGraph(withKernelTestGraph, null, null);
        //    if (!Faster.IsChordal2(noKernelAnalysis, noKernelTestGraph))
        //        throw new Exception("nokernel is not chordal TestGraph10");
        //    if (!Faster.IsChordal2(withKernelAnalysis, withKernelTestGraph))
        //        throw new Exception("withkernel is not chordal TestGraph10");


        //    //TestGraph11
        //    noKernel = Faster.Run(TestGraph11());
        //    withKernel = Faster.RunWithKernel(TestGraph11());
        //    if (noKernel.Item1 != withKernel.Item1)
        //        throw new Exception("different k in kernel test TestGraph11");
        //    noKernelTestGraph = TestGraph11();
        //    withKernelTestGraph = TestGraph11();
        //    noKernelTestGraph.AddEdgeRange(noKernel.Item2);
        //    withKernelTestGraph.AddEdgeRange(withKernel.Item2);
        //    noKernelAnalysis = MoplexAnalysis.AnalyseGraph(noKernelTestGraph, null, null);
        //    withKernelAnalysis = MoplexAnalysis.AnalyseGraph(withKernelTestGraph, null, null);
        //    if (!Faster.IsChordal2(noKernelAnalysis, noKernelTestGraph))
        //        throw new Exception("nokernel is not chordal TestGraph11");
        //    if (!Faster.IsChordal2(withKernelAnalysis, withKernelTestGraph))
        //        throw new Exception("withkernel is not chordal TestGraph11");


        //    //TestGraph12
        //    noKernel = Faster.Run(TestGraph12());
        //    withKernel = Faster.RunWithKernel(TestGraph12());
        //    if (noKernel.Item1 != withKernel.Item1)
        //        throw new Exception("different k in kernel test TestGraph12");
        //    noKernelTestGraph = TestGraph12();
        //    withKernelTestGraph = TestGraph12();
        //    noKernelTestGraph.AddEdgeRange(noKernel.Item2);
        //    withKernelTestGraph.AddEdgeRange(withKernel.Item2);
        //    noKernelAnalysis = MoplexAnalysis.AnalyseGraph(noKernelTestGraph, null, null);
        //    withKernelAnalysis = MoplexAnalysis.AnalyseGraph(withKernelTestGraph, null, null);
        //    if (!Faster.IsChordal2(noKernelAnalysis, noKernelTestGraph))
        //        throw new Exception("nokernel is not chordal TestGraph12");
        //    if (!Faster.IsChordal2(withKernelAnalysis, withKernelTestGraph))
        //        throw new Exception("withkernel is not chordal TestGraph12");

        //    //Test of external graph20.graph
        //    var graph1 = Program.ParseFile(@"..\..\testdata\20.graph", false);
        //    var graph2 = Program.ParseFile(@"..\..\testdata\20.graph", false);
        //    noKernel = Faster.Run(graph1);
        //    withKernel = Faster.RunWithKernel(graph2);
        //    if (noKernel.Item1 != withKernel.Item1)
        //        throw new Exception("different k in kernel test graph20.graph");
        //    noKernelTestGraph = Program.ParseFile(@"..\..\testdata\20.graph", false);
        //    withKernelTestGraph = Program.ParseFile(@"..\..\testdata\20.graph", false);
        //    noKernelTestGraph.AddEdgeRange(noKernel.Item2);
        //    withKernelTestGraph.AddEdgeRange(withKernel.Item2);
        //    noKernelAnalysis = MoplexAnalysis.AnalyseGraph(noKernelTestGraph, null, null);
        //    withKernelAnalysis = MoplexAnalysis.AnalyseGraph(withKernelTestGraph, null, null);
        //    if (!Faster.IsChordal2(noKernelAnalysis, noKernelTestGraph))
        //        throw new Exception("nokernel is not chordal graph20.graph");
        //    if (!Faster.IsChordal2(withKernelAnalysis, withKernelTestGraph))
        //        throw new Exception("withkernel is not chordal graph20.graph");


        //    Console.WriteLine("kernel tests succeeded");

            //v* tests
            var analysis = MoplexAnalysis.AnalyseGraph(TestGraphs.TestGraph2(), null, null);
            var vStar = Faster.FindVStar(new Edge<int>(0, 7), new HashSet<int>(TestGraphs.TestGraph2().Vertices), TestGraphs.TestGraph2());
            if (vStar != 6)
                throw new Exception("Error in VStar");

            analysis = MoplexAnalysis.AnalyseGraph(TestGraphs.TestGraph8(), null, null);
            if (analysis.Moplexes.Count != 5)
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

            analysis = MoplexAnalysis.AnalyseGraph(TestGraphs.TestGraph11(), new List<Edge<int>> { new Edge<int>(0, 4) }, analysis.Moplexes);
            if (analysis.Moplexes.Count != 3)
                throw new Exception("Error in findmoplex without previous");

            //four cycle test
            var a = Faster.FindFourCycle1(TestGraphs.TestGraph2());
            var b = Faster.FindFourCycle2(TestGraphs.TestGraph2());
            var c = Faster.FindFourCycle3BUGGY(TestGraphs.TestGraph2());
            if (a != b && !a.SequenceEqual(b))
                throw new Exception("difference between findFourCycle1 & 2");
            if (a != c && !a.SequenceEqual(c))
                throw new Exception("difference between findFourCycle1 & 3");
            a = Faster.FindFourCycle1(TestGraphs.TestGraph4());
            b = Faster.FindFourCycle2(TestGraphs.TestGraph4());
            c = Faster.FindFourCycle3BUGGY(TestGraphs.TestGraph4());
            if (a != b && !a.SequenceEqual(b))
                throw new Exception("difference between findFourCycle1 & 2");
            if (a != c && !a.SequenceEqual(c))
                throw new Exception("difference between findFourCycle1 & 3");
            a = Faster.FindFourCycle1(TestGraphs.TestGraph5());
            b = Faster.FindFourCycle2(TestGraphs.TestGraph5());
            c = Faster.FindFourCycle3BUGGY(TestGraphs.TestGraph5());
            if (a != b && !a.SequenceEqual(b))
                throw new Exception("difference between findFourCycle1 & 2");
            if (a != c && !a.SequenceEqual(c))
                throw new Exception("difference between findFourCycle1 & 3");
            a = Faster.FindFourCycle1(TestGraphs.TestGraph6());
            b = Faster.FindFourCycle2(TestGraphs.TestGraph6());
            c = Faster.FindFourCycle3BUGGY(TestGraphs.TestGraph6());
            if (a != b && !a.SequenceEqual(b))
                throw new Exception("difference between findFourCycle1 & 2");
            if (a != c && !a.SequenceEqual(c))
                throw new Exception("difference between findFourCycle1 & 3");
            //var g = TestGraph8reorder(); //weirdness test TODO needs fix or convincing argument for why not
            //a = Faster.FindFourCycle1(g);
            //b = Faster.FindFourCycle2(g);
            //c = Faster.FindFourCycle3BUGGY(g);
            //if (a != b && !a.SequenceEqual(b))
            //    throw new Exception("difference between findFourCycle1 & 2");
            //if (a != c && !a.SequenceEqual(c))
            //    throw new Exception("difference between findFourCycle1 & 3");

            a = Faster.FindFourCycle1(TestGraphs.TestGraph8());
            b = Faster.FindFourCycle2(TestGraphs.TestGraph8());
            c = Faster.FindFourCycle3BUGGY(TestGraphs.TestGraph8());
            if (a != b && !a.SequenceEqual(b))
                throw new Exception("difference between findFourCycle1 & 2");
            if (a != c && !a.SequenceEqual(c))
                throw new Exception("difference between findFourCycle1 & 3");
            a = Faster.FindFourCycle1(TestGraphs.TestGraph13());
            b = Faster.FindFourCycle2(TestGraphs.TestGraph13());
            c = Faster.FindFourCycle3BUGGY(TestGraphs.TestGraph13());
            if (a != b && !a.SequenceEqual(b))
                throw new Exception("difference between findFourCycle1 & 2");
            if (a != c && !a.SequenceEqual(c))
                throw new Exception("difference between findFourCycle1 & 3");
            a = Faster.FindFourCycle1(TestGraphs.TestGraph14());
            b = Faster.FindFourCycle2(TestGraphs.TestGraph14());
            c = Faster.FindFourCycle3BUGGY(TestGraphs.TestGraph14());
            if (a != b && !a.SequenceEqual(b))
                throw new Exception("difference between findFourCycle1 & 2");
            if (a != c && !a.SequenceEqual(c))
                throw new Exception("difference between findFourCycle1 & 3");
            a = Faster.FindFourCycle1(TestGraphs.TestGraph15());
            b = Faster.FindFourCycle2(TestGraphs.TestGraph15());
            c = Faster.FindFourCycle3BUGGY(TestGraphs.TestGraph15());
            if (a != b && !a.SequenceEqual(b))
                throw new Exception("difference between findFourCycle1 & 2");
            if (a != c && !a.SequenceEqual(c))
                throw new Exception("difference between findFourCycle1 & 3");


            // Too big k test
            (var k1, var ret1) = Faster.FasterInner(TestGraphs.TestGraph8(), 8, 16, new HashSet<int>(), null, null, new HashSet<Edge<int>>());
            (var k2, var ret2) = Faster.Run(TestGraphs.TestGraph8());
            if ((8 - k1) != k2)
                throw new Exception("Wrong result if k is started too big");
        }

    }
}

