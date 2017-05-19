#include "../CPPace.Lib/GraphIO.h"

using namespace std;

int main(int argc, char *argv[]) {
  switch (argc) {
  case 2: {
    GraphIO::GraphContext context = GraphIO::read_from_path(argv[1]);
    cout << "NUM_EDGES: ..... " << context.graph.num_edges << endl;
    cout << "NUM_VERTICES: .. " << context.graph.num_vertices << endl;
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
