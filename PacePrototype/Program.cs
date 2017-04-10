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
            var analysis = MoplexAnalysis.AnalyseGraph(TestGraphs.TestGraph9());
            var ch = Faster.IsChordal(analysis);
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
