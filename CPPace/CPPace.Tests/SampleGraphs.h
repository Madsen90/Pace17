#pragma once

#include "../CPPace.Lib/AdjacencyList.h"

class SampleGraphs {
public:
  static AdjacencyList berry_bordat() {
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
    return graph;
  }

  // Graph with 3-vertex moplex {0,1,2}, 6-vertex cycle, and "false" moplex triangle
  static AdjacencyList multiple_moplexes_and_big_cc()
  {
    AdjacencyList graph(11);
    graph.add_edge(0, 1);
    graph.add_edge(0, 2);
    graph.add_edge(0, 3);
    graph.add_edge(1, 2);
    graph.add_edge(1, 3);
    graph.add_edge(2, 3);
    graph.add_edge(3, 4);
    graph.add_edge(3, 10);
    graph.add_edge(4, 5);
    graph.add_edge(4, 6);
    graph.add_edge(5, 6);
    graph.add_edge(5, 8);
    graph.add_edge(6, 7);
    graph.add_edge(8, 9);
    graph.add_edge(9, 10);
    return graph;
  }

  //from test graphs 84.graph - exhibits wierd behaviour on kernel
  static AdjacencyList instances_84()
  {
    AdjacencyList graph(76);
    {
      graph.add_edge(8, 31);
      graph.add_edge(16, 57);
      graph.add_edge(75, 33);
      graph.add_edge(49, 0);
      graph.add_edge(16, 71);
      graph.add_edge(36, 60);
      graph.add_edge(7, 62);
      graph.add_edge(20, 66);
      graph.add_edge(4, 33);
      graph.add_edge(3, 21);
      graph.add_edge(17, 66);
      graph.add_edge(14, 55);
      graph.add_edge(27, 32);
      graph.add_edge(45, 39);
      graph.add_edge(8, 36);
      graph.add_edge(0, 57);
      graph.add_edge(7, 58);
      graph.add_edge(3, 68);
      graph.add_edge(27, 51);
      graph.add_edge(50, 46);
      graph.add_edge(0, 71);
      graph.add_edge(7, 34);
      graph.add_edge(32, 51);
      graph.add_edge(7, 29);
      graph.add_edge(42, 66);
      graph.add_edge(37, 10);
      graph.add_edge(26, 56);
      graph.add_edge(11, 45);
      graph.add_edge(18, 12);
      graph.add_edge(70, 33);
      graph.add_edge(64, 69);
      graph.add_edge(64, 21);
      graph.add_edge(64, 65);
      graph.add_edge(24, 68);
      graph.add_edge(74, 12);
      graph.add_edge(47, 26);
      graph.add_edge(38, 73);
      graph.add_edge(31, 24);
      graph.add_edge(13, 56);
      graph.add_edge(23, 9);
      graph.add_edge(11, 67);
      graph.add_edge(48, 33);
      graph.add_edge(63, 43);
      graph.add_edge(7, 33);
      graph.add_edge(2, 14);
      graph.add_edge(16, 64);
      graph.add_edge(60, 5);
      graph.add_edge(11, 18);
      graph.add_edge(73, 25);
      graph.add_edge(48, 44);
      graph.add_edge(63, 19);
      graph.add_edge(64, 22);
      graph.add_edge(50, 39);
      graph.add_edge(20, 6);
      graph.add_edge(17, 26);
      graph.add_edge(44, 37);
      graph.add_edge(45, 23);
      graph.add_edge(63, 6);
      graph.add_edge(54, 42);
      graph.add_edge(67, 61);
      graph.add_edge(20, 56);
      graph.add_edge(28, 33);
      graph.add_edge(15, 33);
      graph.add_edge(59, 42);
      graph.add_edge(73, 16);
      graph.add_edge(16, 1);
      graph.add_edge(46, 39);
      graph.add_edge(13, 52);
      graph.add_edge(53, 3);
      graph.add_edge(72, 30);
      graph.add_edge(47, 41);
      graph.add_edge(44, 72);
      graph.add_edge(64, 35);
      graph.add_edge(19, 49);
      graph.add_edge(41, 43);
      graph.add_edge(19, 0);
      graph.add_edge(6, 0);
      graph.add_edge(3, 5);
      graph.add_edge(61, 74);
      graph.add_edge(11, 61);
      graph.add_edge(23, 50);
      graph.add_edge(41, 63);
      graph.add_edge(2, 40);
    }
    return graph;
  }
};
