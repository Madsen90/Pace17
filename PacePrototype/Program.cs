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
            Faster.AnalyseGraph(Faster.TestGraph1());
            Faster.AnalyseGraph(Faster.TestGraph2());
            Faster.AnalyseGraph(Faster.TestGraph3());
        }
    }
}
