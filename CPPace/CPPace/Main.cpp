#include "../CPPace.Lib/GraphIO.h"

using namespace std;

int main(int argc, char *argv[]) {
  switch (argc) {
  case 2: {
    GraphContext context = GraphIO::read_from_path(argv[1]);
    cout << "NUM_EDGES: ..... " << context.graph.num_edges << endl;
    cout << "NUM_VERTICES: .. " << context.graph.num_vertices << endl;
    break;
  }
  default:
    cout << "Invalid arguments" << endl;
    break;
  }
}
