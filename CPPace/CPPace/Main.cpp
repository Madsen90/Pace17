#include "../CPPace.Lib/Log.h"
#include "../CPPace.Lib/GraphIO.h"
#include "../CPPace.Lib/MinimumFillIn.h"

using namespace std;

int main(int argc, char *argv[]) {
  switch (argc) {
  case 2: {
    GraphIO::GraphContext context = GraphIO::read_from_path(argv[1]);
    Log::info("NUM_EDGES: ..... " + to_string(context.graph.num_edges));
    Log::info("NUM_VERTICES: .. " + to_string(context.graph.num_vertices));
    stack<pair<int, int>> edges = MinimumFillIn::minimum_fill_in(context);
    while (!edges.empty()) {
      pair<int, int> edge = edges.top();
      edges.pop();
      Log::info(to_string(edge.first) + " " + to_string(edge.second));
    }
    Log::info("Done");
    break;
  }
  default:
    Log::error("Invalid arguments");
    break;
  }
}
