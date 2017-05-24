#include "../CPPace.Lib/Log.h"
#include "../CPPace.Lib/GraphIO.h"

using namespace std;

int main(int argc, char *argv[]) {
  switch (argc) {
  case 2: {
    GraphIO::GraphContext context = GraphIO::read_from_path(argv[1]);
    Log::info("NUM_EDGES: ..... " + to_string(context.graph.num_edges));
    Log::info("NUM_VERTICES: .. " + to_string(context.graph.num_vertices));
    break;
  }
  default:
    Log::error("Invalid arguments");
    break;
  }
}
