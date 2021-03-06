#include "../CPPace.Lib/Log.h"
#include "../CPPace.Lib/GraphIO.h"
#include "../CPPace.Lib/MinimumFillIn.h"
#include "../CPPace.Lib/MCS.h"

using namespace std;

int main(int argc, char *argv[]) {
#ifdef OPTIL
  GraphIO::GraphContext context = GraphIO::read_from_stream(cin);
  stack<pair<int, int>> edges = MinimumFillIn::minimum_fill_in(context);
  while (!edges.empty()) {
    pair<int, int> edge = edges.top();
    edges.pop();
    cout << context.vertex_name(edge.first) << " " << context.vertex_name(edge.second) << endl;
  }
  exit(0);
#endif
  
  switch (argc) {
  case 2: {
    GraphIO::GraphContext context = GraphIO::read_from_path(argv[1]);
    Log::info("NAME: .......... %s", argv[1]);
    Log::info("NUM_EDGES: ..... %d", context.graph.num_edges);
    Log::info("NUM_VERTICES: .. %d", context.graph.num_vertices);
    Log::info("========================================");
    
    stack<pair<int, int>> edges = MinimumFillIn::minimum_fill_in(context);    
    stack<pair<int, int>> test_edges(edges);
    while(!test_edges.empty()) {
      pair<int, int> edge = test_edges.top();
      test_edges.pop();
      context.graph.add_edge(edge.first, edge.second);
    }
    if (!MCS::is_chordal(context.graph)) {
      Log::error("Error in result!");
    }
    Log::info("Solution:");
    while (!edges.empty()) {
      pair<int, int> edge = edges.top();
      edges.pop();
      Log::info("%s -- %s", context.vertex_name(edge.first).c_str(), context.vertex_name(edge.second).c_str());
    }
    break;
  }
  default:
    Log::error("Invalid arguments");
    break;
  }
}
