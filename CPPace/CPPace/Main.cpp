#include "../CPPace.Lib/GraphIO.h"
#include "../CPPace.Lib/Kernelizer.h"

using namespace std;

void print_kernel_info(const Kernel& kernel)
{
  cout << "a: ";
  for (int a : kernel.a)
    cout << a << ", ";
  cout << endl;
  cout << "b: ";
  for (int b : kernel.b)
    cout << b << ", ";
  cout << endl;
  cout << "kMin: " << kernel.kMin << endl;
  cout << "essential edges:\n";
  for (pair<int, int> ee : kernel.essential_edges)
    cout << "(" << ee.first << ", " << ee.second << ")";
  cout << endl;
}

int main(int argc, char *argv[]) {
  switch (argc) {
  case 3: {
    GraphIO::GraphContext context = GraphIO::read_from_path(argv[1]);
    cout << "NUM_EDGES: ..... " << context.graph.num_edges << endl;
    cout << "NUM_VERTICES: .. " << context.graph.num_vertices << endl;
    cout << "phase1:" << endl;
    Kernel k1 = Kernelizer::phase1(context.graph);
    cout << "phase2:" << endl;
    Kernelizer::phase2(context.graph, k1);
    print_kernel_info(k1);
    Kernel k2;
    cout << "phase3:" << endl;
    Kernelizer::phase3(context.graph, k1, k2, atoi(argv[2]));
    print_kernel_info(k2);
    break;
  }
  case 1: {
    AdjacencyList graph(8);
    graph.add_edge(0, 1);
    graph.add_edge(0, 2);
    graph.add_edge(0, 4);
    graph.add_edge(0, 5);
    graph.add_edge(1, 2);
    graph.add_edge(1, 4);
    graph.add_edge(1, 5);
    graph.add_edge(2, 5);
    graph.add_edge(2, 6);
    graph.add_edge(3, 5);
    graph.add_edge(3, 6);
    graph.add_edge(4, 7);
    graph.add_edge(5, 7);
    graph.add_edge(6, 7);

    GraphIO::GraphContext context(graph);
    context.vertex_color_map["red"] = set<int>{ 1, 3, 5 };
    context.edge_color_map["red"] = set<pair<int, int>> {
      pair<int, int>(1, 3),
      pair<int, int>(3, 5)
    };
    GraphIO::write_to_path(context, "out.dot", GraphIO::DOT);
    break;
  }
  default:
    cout << "Invalid arguments" << endl;
    break;
  }
}
