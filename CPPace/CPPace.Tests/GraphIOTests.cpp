#include "stdafx.h"
#include "CppUnitTest.h"
#include "../CPPace.Lib/GraphIO.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace CPPaceTests {
  TEST_CLASS(GraphIOTests) {
  public:
    TEST_METHOD(Squash) {
      GraphIO::GraphContext context = GraphIO::GraphContext(SampleGraphs::berry_bordat());
      context.vertex_color_map["red"].emplace(0);
      context.vertex_color_map["red"].emplace(1);
      context.vertex_color_map["red"].emplace(2);
      context.vertex_color_map["green"].emplace(4);
      context.vertex_color_map["blue"].emplace(5);
      context.vertex_color_map["blue"].emplace(6);
      
      context.edge_color_map["red"].emplace(0, 1);
      context.edge_color_map["red"].emplace(0, 2);
      context.edge_color_map["red"].emplace(1, 2);
      context.edge_color_map["green"].emplace(0, 4);
      context.edge_color_map["blue"].emplace(5, 6);

      context.graph.remove_vertex(4);
      GraphIO::GraphContext squash1 = context.squash();
      Assert::AreEqual(7, squash1.graph.num_vertices);
      Assert::AreEqual(11, squash1.graph.num_edges);
      Assert::IsTrue(set<int> { 0, 1, 2 } == squash1.vertex_color_map["red"]);
      Assert::IsTrue(set<int> {} == squash1.vertex_color_map["green"]);
      Assert::IsTrue(set<int> { 4, 5 } == squash1.vertex_color_map["blue"]);
      Assert::IsTrue(set<pair<int, int>> {
        pair<int, int>(0, 1),
          pair<int, int>(0, 2),
          pair<int, int>(1, 2)
      } == squash1.edge_color_map["red"]);
      Assert::IsTrue(set<pair<int, int>> {} == squash1.edge_color_map["green"]);
      Assert::IsTrue(set<pair<int, int>> { pair<int, int>(4, 5) } == squash1.edge_color_map["blue"]);
      Assert::AreEqual(string("0"), squash1.vertex_name(0));
      Assert::AreEqual(string("1"), squash1.vertex_name(1));
      Assert::AreEqual(string("2"), squash1.vertex_name(2));
      Assert::AreEqual(string("3"), squash1.vertex_name(3));
      Assert::AreEqual(string("5"), squash1.vertex_name(4));
      Assert::AreEqual(string("6"), squash1.vertex_name(5));
      Assert::AreEqual(string("7"), squash1.vertex_name(6));

      squash1.graph.remove_vertices(set<int> { 0, 2 });
      GraphIO::GraphContext squash2 = squash1.squash();
      Assert::AreEqual(5, squash2.graph.num_vertices);
      Assert::AreEqual(5, squash2.graph.num_edges);
      Assert::IsTrue(set<int> { 0 } == squash2.vertex_color_map["red"]);
      Assert::IsTrue(set<int> {} == squash2.vertex_color_map["green"]);
      Assert::IsTrue(set<int> { 2, 3 } == squash2.vertex_color_map["blue"]);
      Assert::IsTrue(set<pair<int, int>> {} == squash2.edge_color_map["red"]);
      Assert::IsTrue(set<pair<int, int>> {} == squash2.edge_color_map["green"]);
      Assert::IsTrue(set<pair<int, int>> { pair<int, int>(2, 3) } == squash2.edge_color_map["blue"]);
      Assert::AreEqual(string("1"), squash2.vertex_name(0));
      Assert::AreEqual(string("3"), squash2.vertex_name(1));
      Assert::AreEqual(string("5"), squash2.vertex_name(2));
      Assert::AreEqual(string("6"), squash2.vertex_name(3));
      Assert::AreEqual(string("7"), squash2.vertex_name(4));
    }
  };
}
