using QuickGraph;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PacePrototype
{
    class Program
    {
        static void Main(string[] args)
        {
            var analysis = MoplexAnalysis.AnalyseGraph(TestGraphs.TestGraph2());
            var vStar = Faster.FindVStar(new Edge<int>(0, 7), new HashSet<int>(TestGraphs.TestGraph2().Vertices), TestGraphs.TestGraph2());
            var k = Faster.Run(TestGraphs.TestGraph8());

            

            MoplexAnalysis.AnalyseGraph(TestGraphs.TestGraph1());
            MoplexAnalysis.AnalyseGraph(TestGraphs.TestGraph2());
            MoplexAnalysis.AnalyseGraph(TestGraphs.TestGraph3());
            MoplexAnalysis.AnalyseGraph(TestGraphs.TestGraph4());
            var a = Faster.FindFourCycle(TestGraphs.TestGraph2());
            a = Faster.FindFourCycle(TestGraphs.TestGraph4());
            a = Faster.FindFourCycle(TestGraphs.TestGraph5());
            a = Faster.FindFourCycle(TestGraphs.TestGraph6());
        }

    }
}
