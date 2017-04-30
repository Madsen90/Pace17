#include "../CPPace.Lib/GraphIO.h"

using namespace std;

int main(int argc, char *argv[]) {
  switch (argc) {
  case 2: {
    AdjacencyList graph = GraphIO::read_from_path(argv[1]);
    cout << "NUM_EDGES: ..... " << graph.num_edges << endl;
    cout << "NUM_VERTICES: .. " << graph.num_vertices << endl;
    break;
  }
  default:
    cout << "Invalid arguments" << endl;
    break;
  }
}
