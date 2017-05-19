using System;
using System.Collections.Generic;
using C5;
using System.Collections.ObjectModel;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using QuickGraph;

namespace PacePrototype
{
    using HashSet = System.Collections.Generic.HashSet<string>;

    public class Pair
    {
        public string X { get; set; }
        public string Y { get; set; }

        public Pair(string x, string y)
        {
            X = x;
            Y = y;
        }

        public override string ToString() => $"({X}, {Y})";
    }


    public class Holder
    {
        //Should use getter and setters, but no thanks.
        public Dictionary<string, List<string>> Graph;

        public HashSet A;
        public HashSet B;
        public int CC;
        public bool Stop = false;
        public List<Pair> EssentialEdges = new List<Pair>();

        public Holder(Dictionary<string, List<string>> graph, HashSet b, HashSet a, int cc)
        {
            Graph = graph;
            A = a;
            B = b;
            CC = cc;
        }

        public Holder(Dictionary<string, List<string>> graph, HashSet b, HashSet a) : this(graph, b, a, 0) { }
        
    }

    class Kernel
    {

        // Takes in a .graph file and reads it - if no file is specified reads from System.in

        private static Holder Phase1(Holder holder)
        {
            var tmpGraph = new Dictionary<string, List<string>>(holder.Graph);
            while (true)
            {
                var res = MCS.ExtractChordlessCycle(tmpGraph);

                //Our break condition
                if (res.Count < 1) break;
                holder.CC += (res.Count - 3);

                tmpGraph = CleanGraph(tmpGraph, holder, res);
            }

            return holder;
        }

        private static Dictionary<string, List<string>> CleanGraph(Dictionary<string, List<string>> graph, Holder holder, List<string> nodes)
        {
            foreach (var s in nodes)
            {
                holder.B.Remove(s);
                holder.A.Add(s);

                graph.Remove(s);
                foreach (var key in graph.Keys)
                {
                    graph[key].Remove(s);
                }
            }
            return graph;
        }

        private static List<string> BFS(string toFind, Holder holder, string startPoint, List<string> listB, List<string> listA)
        {
            var q = new IntervalHeap<string>();
            var distTo = new Dictionary<string, int>();
            var edgeTo = new Dictionary<string, string>();
            var marked = new Dictionary<string, bool>();
            var newGraph = Copy(holder.Graph);
            var yEdges = newGraph[toFind];
            var xEdges = newGraph[startPoint];
            //removing all connections to the a set
            for (int i = 0; i < yEdges.Count; i++)
            {
                var node = yEdges[i];
                if (listA.Contains(node))
                {
                    var tmp = newGraph[node];
                    tmp.Remove(toFind);
                    newGraph[node] = tmp;
                }
                else if (xEdges.Contains(node))
                {
                    var tmp = newGraph[node];
                    tmp.Remove(toFind);
                    tmp.Remove(startPoint);
                    newGraph[node] = tmp;
                    xEdges.Remove(node);
                }

            }
            foreach (var s in listA)
            {
                yEdges.Remove(s);
            }
            newGraph[toFind] = yEdges;
            newGraph[startPoint] = xEdges;

            //We just start at the first node
            q.Add(startPoint);
            //We now just keep track of occurrences
            //Should be two consecutive nodes? This does not do that currently
            //Unsure of examples where this would fail
            distTo[startPoint] = 0;
            marked[startPoint] = true;
            while (!q.IsEmpty)
            {
                var v = q.DeleteMin();
                foreach (var w in newGraph[v])
                {
                    if (marked.ContainsKey(w))
                    {
                        var mark = marked[w];
                        if (mark) continue;
                    }

                    marked[w] = true;
                    distTo[w] = distTo[v] + 1;
                    edgeTo[w] = v;
                    q.Add(w);
                }
            }
            // Unsure if I need this
            // int conBs = 0;
            var path = new List<string>();
            if (marked.ContainsKey(toFind))
            {
                string x;
                for (x = toFind; distTo[x] != 0; x = edgeTo[x])
                {
                    path.Add(x);
                }
                path.Add(x);
            }
            return path;

        }
        
        //Missing increasing CC
        private static Holder Phase2(Holder holder)
        {
            var listA = holder.A.ToList();
            var listB = holder.B.ToList();
            //I could not do this with a set, but can with a list.
            //It makes sense, thinking about the structure of Java, that I
            //can do this, but not very safe, might be an endless loop!!!
            for (var i = 0; i < listA.Count; i++)
            {
                var x = listA[i];
                //Detect if elements are in B, if they are do stuff
                var edges = holder.Graph[x];
                //Do we want to add things we discover runningly or not?

                for (var j = 0; j < edges.Count; j++)
                {
                    var t = edges[j];
                    if (!listB.Contains(t)) continue;
                    //shouldBeAdded = true;
                    //Found our Y
                    var y = t;

                    //Need BFS here -- Otherwise it is impossible to locate
                    //whether or not neighbour to look at is present in a cycle.
                    //Looking at neighbours of Y, to see if they are connected to
                    //at least one neighbour of X.
                    var yNeighbours = new List<string>(holder.Graph[y]);
                    //Remove all common neighbours
                    if (yNeighbours.Count <= 0) continue;

                    foreach (var neighbour in yNeighbours)
                    {
                        //for each neighbour
                        var path = BFS(neighbour, holder, x, listB, listA);
                        //Found a cycle
                        if (path.Count > 0)
                        {
                            var subpaths = new List<int>();
                            var currentSubPath = 0;
                            var foundPath = false;
                            foreach (var s in path)
                            {
                                if (listB.Contains(s))
                                {
                                    foundPath = true;
                                    currentSubPath++;
                                    listA.Add(s);
                                    listB.Remove(s);
                                }
                                else
                                {
                                    if (foundPath)
                                    {
                                        if (currentSubPath - 1 > 0) subpaths.Add(currentSubPath - 1);
                                        foundPath = false;
                                        currentSubPath = 0;
                                    }
                                }
                            }
                            if (subpaths.Count > 0)
                            {
                                if (subpaths.Count == 1)
                                {
                                    var subpath = subpaths[0];

                                    holder.CC += path.Count - subpath == 1 ? subpath - 2 : subpath - 1;
                                }
                                else
                                {
                                    var addToCC = subpaths.Sum(p => p - 1);
                                    holder.CC += (addToCC / 2);
                                }
                            }
                        }
                    }
                }
            }
            holder.A = new HashSet(listA);
            holder.B = new HashSet(listB);
            return holder;
        }


        private static List<string> BFSPhase3(string toFind, Dictionary<string, List<string>> newGraph, string startPoint)
        {
            var q = new IntervalHeap<string>();
            var distTo = new Dictionary<string, int>();
            var edgeTo = new Dictionary<string, string>();
            var marked = new Dictionary<string, bool>();
            
            //We just start at the first node
            q.Add(startPoint);
            //We now just keep track of occurrences
            //Should be two consecutive nodes? This does not do that currently
            //Unsure of examples where this would fail
            distTo.Add(startPoint, 0);
            marked.Add(startPoint, true);
            var counter = 0;
            while (!q.IsEmpty)
            {
                var v = q.DeleteMin();
                foreach (var w in newGraph[v])
                {
                    if (w.Equals(toFind)) counter++;
                    var mark = false;
                    if (marked.ContainsKey(w))
                        mark = marked[w];
                    if (!mark)
                    {
                        marked[w] = true;
                        distTo[w] = distTo[v] + 1;
                        edgeTo[w] = v;
                        q.Add(w);
                    }
                }
            }


            var path = new List<string>();
            var gotThere = marked[toFind];
            if (gotThere)
            {
                string x;
                for (x = toFind; distTo[x] != 0; x = edgeTo[x])
                {
                    path.Add(x);
                }
                path.Add(x);
            }
            //Now path should contain the shortest possible path;
            //Is not part of a cycle
            return path;

        }

        public static Holder Phase3(Holder holder, int k)
        {
            var startGraph = Copy(holder.Graph);
            var listA = new List<string>(holder.A);
            var listB = new List<string>(holder.B);
            //First generate all nonadjacent pairs from A
            var pairs = new List<Pair>();
            var resolvedNodes = new List<string>();
            foreach (var v in holder.A)
            {
                var adjacent = holder.Graph[v];
                var tmpAs = new HashSet();
                tmpAs.UnionWith(holder.A);
                tmpAs.UnionWith(adjacent);
                foreach (var s in tmpAs)
                {
                    //We can make this check as every pair should have already been made, if
                    //the given node is found in resolvedNodes.
                    if (s.Equals(v)) continue;
                    if (!resolvedNodes.Contains(s))
                    {
                        pairs.Add(new Pair(v, s));
                    }
                }
                resolvedNodes.Add(v);
            }

            //First we found all candidates
            foreach (var pair in pairs)
            {
                //To stick with the notation of the paper, y is x, and z is y form the pair.
                var yNeighbours = startGraph[pair.X];
                //The set we need to check whether is larger than 2k.
                var candidates = new List<string>();
                foreach (var yNeighbour in yNeighbours)
                {
                    if (listB.Contains(yNeighbour) && startGraph[yNeighbour].Contains(pair.Y))
                    {
                        var graph = Copy(startGraph);
                        var neighbourEdges = graph[yNeighbour];
                        foreach (var edge in neighbourEdges)
                        {
                            if (edge.Equals(pair.X) || edge.Equals(pair.Y)) continue;
                            var tmp = graph[edge];
                            tmp.Remove(yNeighbour);
                            graph[edge] = tmp;
                        }
                        var newEdges = new List<string> {pair.X, pair.Y};

                        graph[yNeighbour] = newEdges;
                        var neighbourPath = BFSPhase3(pair.Y, graph, yNeighbour);
                        if (neighbourPath.Count > 0) candidates.Add(yNeighbour);
                    }
                }
                if (candidates.Count > 2 * k)
                {
                    holder.EssentialEdges.Add(pair);
                }
                else
                {
                    foreach (var s in candidates)
                    {
                        listB.Remove(s);
                        listA.Add(s);
                    }
                }
            }

            holder.A = new HashSet(listA);
            holder.B = new HashSet(listB);

            return holder;
        }


        private Dictionary<string, List<string>> graphStart;
        private Dictionary<string, List<string>> graph1;
        private Dictionary<string, List<string>> graph2;
        private Dictionary<string, List<string>> graph3;
        private Holder _holder;

        public Kernel(Dictionary<string, List<string>> graph)
        {
            graphStart = Copy(graph);
            graph1 = Copy(graph);
            graph2 = Copy(graph);
            graph3 = Copy(graph);
            _holder = new Holder(graph1, new HashSet(graph.Keys), new HashSet());
        }

        public Holder RunPhase1Then2()
        {
            RunPhase1();
            return RunPhase2();
        }

        public Holder RunPhase1()
        {
            _holder = Phase1(_holder);
            _holder.Graph = graph2;
            return _holder;
        }

        public Holder RunPhase2()
        {
            _holder = Phase2(_holder);
            _holder.Graph = graph3;
            return _holder;
        }

        public Holder RunPhase3(int k)
        {
            _holder = Phase3(_holder, k);
            _holder.Graph = graphStart;
            return _holder;
        }


        public static Dictionary<string, List<string>> Copy(Dictionary<string, List<string>> original)
        {
            return original.Keys.ToDictionary(key => key, key => new List<string>(original[key]));
        }

        public static Holder Init(UndirectedGraph<int, Edge<int>> graph)
        {
            var graph1 = new Dictionary<string, List<string>>();
            foreach (var v in graph.Vertices)
            {
                graph1.Add(v.ToString(), graph.AdjacentEdges(v).Select(e => e.GetOtherVertex(v).ToString()).ToList());
            }

            var kernel = new Kernel(graph1);
            return kernel.RunPhase1Then2();

        }

        //public static void main(string[] args)
        //{
        //    InputStream input;
        //    if (args.length == 0)
        //        input = System.in;
        //    else
        //        input = new FileInputStream(new File(args[0]));

        //    try (Scanner scanner = new Scanner(input)) {
        //        Dictionary<string, List<string>> graph = new Dictionary<>();
        //        Dictionary<string, List<string>> graph2 = new Dictionary<>();
        //        Dictionary<string, List<string>> graph3 = new Dictionary<>();
        //        while (scanner.hasNextLine())
        //        {
        //            string line = scanner.nextLine();

        //            string[] split = line.split(" ");
        //            if (graph.containsKey(split[0]))
        //            {
        //                graph[split]0]).Add(split[1]);
        //                graph2[split]0]).Add(split[1]);
        //                graph3[split]0]).Add(split[1]);
        //            }
        //            else
        //            {
        //                List<string> list = new List<>();
        //                List<string> list2 = new List<>();
        //                List<string> list3 = new List<>();
        //                list.Add(split[1]);
        //                list2.Add(split[1]);
        //                list3.Add(split[1]);
        //                graph.Add(split[0], list);
        //                graph2.Add(split[0], list2);
        //                graph3.Add(split[0], list3);
        //            }
        //            if (graph.containsKey(split[1]))
        //            {
        //                graph[split]1]).Add(split[0]);
        //                graph2[split]1]).Add(split[0]);
        //                graph3[split]1]).Add(split[0]);
        //            }
        //            else
        //            {
        //                List<string> list = new List<>();
        //                List<string> list2 = new List<>();
        //                List<string> list3 = new List<>();
        //                list.Add(split[0]);
        //                list2.Add(split[0]);
        //                list3.Add(split[0]);
        //                graph.Add(split[1], list);
        //                graph2.Add(split[1], list2);
        //                graph3.Add(split[1], list3);
        //            }
        //        }
        //        Kernel kernel = new Kernel(graph);
        //        kernel.RunPhase1Then2();
        //        Holder holder = kernel.RunPhase3(1000);
        //        //No idea what this value should really be at this time.
        //        //int k = 100000;
        //        //  Holder holder = new Holder(new Dictionary(graph), graph.keySet(), new HashHashSet(), 0, k);
        //        //More functional approach, dunno if this is a performance issue
        //        /*holder = Phase1(holder);
        //        if(holder.stop) return;
        //        holder.graph = graph2;//new Dictionary(graph);//graph2;
        //        holder = Phase2(holder);
        //        if(holder.stop) return;
        //        holder.graph = new Dictionary(graph3);//graph3;
        //        holder = Phase3(holder);
        //        if(holder.stop) return;*/
        //        Console.WriteLine(holder.A);
        //        Console.WriteLine(holder.B);
        //        Console.WriteLine(holder.cc);
        //        Console.WriteLine(holder.essentialEdges);


        //        //  Kernel kernel = new Kernel(graph);
        //        //  kernel.runStuff();
        //        //  Console.WriteLine(holder.graph);
        //        //Console.WriteLine(holder.A.Count);


        //        /*
        //        bool hasChanged = true;
        //        while(hasChanged)
        //        {
        //            hasChanged = false;
        //            for (Dictionary.Entry<string, List<string>> vertexAndEdges : graph.entrySet()) {
        //                if(vertexAndEdges.getValue().Count==1)
        //                {
        //                    string other = vertexAndEdges.getValue()[0];
        //                    graph[other].Remove(vertexAndEdges.getKey());
        //                    vertexAndEdges.getValue().Remove(other);
        //                    hasChanged = true;
        //                }
        //            }
        //        }*/
        //        /*
        //        HashHashSet outputted = new HashHashSet<>();
        //        for (Dictionary.Entry<string, List<string>> vertexAndEdges : graph.entrySet()) {
        //            for (string s : vertexAndEdges.getValue()) {
        //                if(!outputted.Contains(s)){
        //                    Console.WriteLine(vertexAndEdges.getKey() + " " + s);
        //                    outputted.Add(vertexAndEdges.getKey());
        //                }
        //            }
        //        }*/
        //    }
    }

    public class MCS
    {

        //Returns empty list if no cycle is found.
        public static List<string> ExtractChordlessCycle(Dictionary<string, List<string>> graph)
        {
            var dim = graph.Count;

            var ordering = new Dictionary<int, string>();
            foreach (var node in graph.Keys)
            {
                ordering.Add(ordering.Count, node);
            }

            for (var i = 0; i < dim - 2; i++)
            {
                for (var j = i + 1; j < dim - 1; j++)
                {
                    if (!AreAdjacent(graph, ordering, i, j))
                    {
                        continue;
                    }

                    for (int k = i + 1; k < dim; k++)
                    {
                        var candidates = new List<List<int>>();

                        for (int l = i + 1; l < dim; l++)
                        {
                            if (l == k || j == k || j == l)
                            {
                                continue;
                            }
                            if (!AreAdjacent(graph, ordering, k, l))
                            {
                                continue;
                            }

                            if (!AreAdjacent(graph, ordering, j, l) && !AreAdjacent(graph, ordering, j, k))
                            {
                                continue;
                            }

                            var nodes = new List<int>();
                            nodes.Add(i);
                            nodes.Add(j);
                            nodes.Add(k);
                            nodes.Add(l);
                            if (IsChordless(graph, ordering, nodes))
                            {
                                var result = new List<string>();
                                result.Add(ordering[i]);
                                result.Add(ordering[j]);
                                result.Add(ordering[k]);
                                result.Add(ordering[l]);
                                return result;
                            }
                            candidates.Add(nodes);
                        }

                        while (candidates.Count > 0)
                        {
                            List<int> v = candidates[0];
                            candidates.RemoveAt(0);

                            int l = v[v.Count - 1];
                            for (int m = i + 1; m < dim; m++)
                            {
                                if (v.Contains(m))
                                {
                                    continue;
                                }
                                if (!AreAdjacent(graph, ordering, m, l))
                                {
                                    continue;
                                }

                                bool chord = false;
                                for (int n = 1; n < v.Count - 1; n++)
                                {
                                    if (AreAdjacent(graph, ordering, m, v[n]))
                                    {
                                        chord = true;
                                    }
                                }
                                if (chord)
                                {
                                    continue;
                                }
                                if (AreAdjacent(graph, ordering, m, k))
                                {
                                    var result = new List<string>();
                                    for (int n = 0; n < v.Count; n++)
                                    {
                                        result.Add(ordering[v[n]]);
                                    }
                                    result.Add(ordering[m]);
                                    return result;
                                }
                                var w = new List<int>(v);
                                w.Add(m);
                                candidates.Add(w);
                            }
                        }
                    }
                }
            }

            return new List<string>();
        }

        private static bool AreAdjacent(Dictionary<string, List<string>> graph, Dictionary<int, string> ordering, int x, int y)
        {
            return graph[ordering[x]].Contains(ordering[y]);
        }

        private static bool IsChordless(Dictionary<string, List<string>> graph, Dictionary<int, string> ordering, List<int> nodes)
        {
            // check if the length of nodes is 4
            if (nodes.Count < 4) return false;
            // create a set from the nodes and see it the length is 4
            var nodeSet = new System.Collections.Generic.HashSet<int>();
            foreach (var node in nodes)
            {
                nodeSet.Add(node);
            }
            if (nodeSet.Count != nodes.Count) return false;
            // for each node count how many adj are there
            foreach (int i in nodes)
            {
                int counter = 0;
                foreach (int j in nodes)
                {
                    if (AreAdjacent(graph, ordering, i, j)) counter++;
                }

                if (counter != 2) return false;
            }
            // if all of them are 2 return true else false
            return true;
        }

        private static Dictionary<string, List<string>> Clone(Dictionary<string, List<string>> original)
        {
            var clone = new Dictionary<string, List<string>>();
            foreach (var kvPair in original)
            {
                clone.Add(kvPair.Key, new List<string>(kvPair.Value));
            }
            return clone;
        }
    }
}
