using QuickGraph;
using QuickGraph.Graphviz;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using QuickGraph.Graphviz.Dot;
using System.IO;

namespace PacePrototype
{
    class DrawGraph
    {
        public static void drawGraph(UndirectedGraph<int, Edge<int>> retGraph, HashSet<Edge<int>> edgeSet, string path)
        {
            var dotGraph = "graph G {\n";
            foreach (var v in retGraph.Vertices)
            {
                dotGraph += $"{v} ;\n";
            }
            foreach (var e in retGraph.Edges)
            {
                dotGraph += $"{e.Source} -- {e.Target} [";
                if (edgeSet.Contains(e))
                    dotGraph += "color=\"red\"";
                dotGraph += "];\n";
            }
            File.WriteAllText(path, dotGraph+"}");
        }

    }
}
