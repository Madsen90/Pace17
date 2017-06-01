#include "../CPPace.Lib/Log.h"
#include "../CPPace.Lib/GraphIO.h"
#include "../CPPace.Lib/MinimumFillIn.h"
#include <filesystem>
#include <thread>
#include "../CPPace.Lib/MCS.h"
#include <future>
#include <random>


using namespace std;
namespace fs = std::experimental::filesystem;

class BoolHolder
{
public:
  bool b = false;
};


void thread_version(GraphIO::GraphContext& context, Terminator* t, promise<stack<pair<int, int>>> && edges)
{
  auto res = MinimumFillIn::minimum_fill_in(context, t);
  if(!t->terminate)
    edges.set_value(res);
}


int main(int argc, char *argv[]) {
  string path = string(argv[1]) + "\\";
  int i = 1; 
  clock_t start = clock();
  for(auto & a : experimental::filesystem::directory_iterator(path)) {
    printf("\n");
    printf("Seconds since start %d\n",clock()/CLOCKS_PER_SEC);
    printf("Starting new minfill, for %s.\n", (a.path().filename().string()));
    
    clock_t begin = clock();
    GraphIO::GraphContext context = GraphIO::read_from_path(path + (a.path().filename().string()));
    GraphIO::write_to_path(context, "C:\\Users\\Frederik\\Downloads\\instances1\\9.dot", GraphIO::DOT);
    Terminator* term = new Terminator();
    promise<bool> pf;
    promise<stack<pair<int, int>>> pe;
    auto edges = pe.get_future();
    thread t = thread(&thread_version, context, term, move(pe));
    while (edges.wait_for(1min) != future_status::ready) {
      if ((clock() - begin) / CLOCKS_PER_SEC > 121) {
        printf("Aborting thread due to time limit.\n");
        term->terminate = true;
        break;
      }
    }
    t.join();
    if (term->terminate) {
      printf("thread aborted\n");
      printf("Found no solution for %s\n", (a.path().filename().string()));
    }
    else {
      printf("thread finished\n");
      stack<pair<int, int>> return_edges = edges.get();
      int size = return_edges.size();
      while(!return_edges.empty()) {
        auto edge = return_edges.top();
        return_edges.pop();
        context.graph.add_edge(edge.first, edge.second);
      }
      if (MCS::is_chordal(context.graph)) {
        printf("Found solution for %s with k=%d\n", (a.path().filename().string()), size);
      }
      else {
        printf("ERROR! Got wrong solution to %s\n", (a.path().filename().string()));
        GraphIO::write_to_path(context, "C:\\Users\\Frederik\\Downloads\\instances1\\9-done.dot", GraphIO::DOT);

      }
    }
    fflush(stdout);
  }
/*
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
  }*/
}
