using QuickGraph;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PacePrototype
{
    class TestGraphs
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

        // one four cycle
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

        // two four cycles
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

        // one four cycle and one five cycle
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

        //chordal graph
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

        // letter graph missing two diagonal edges
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

        // Graph 8 as chordal
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
    }
}

