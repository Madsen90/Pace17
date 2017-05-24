#include "stdafx.h"
#include "CppUnitTest.h"
#include "../CPPace.Lib/LexBFS.h"
#include "../CPPace.Lib/Kernelizer.h"
#include <iostream>
#include "../CPPace.Lib/MCS.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace CPPaceTests {
  TEST_CLASS(MCSLexBFSComparison) {
public:

  AdjacencyList BuildNFully(int n) {
    //Fully connected with N nodes
    AdjacencyList graph(n);
    for (int i = 0; i < n; i++) {
      for (int j = 1; j < n; j++) {
        graph.add_edge(i, j);
      }
    }
    return graph;
  }

  AdjacencyList BuildNFullyHalfDeleted(int n) {
    //Fully connected with N nodes
    AdjacencyList graph(n);
    for (int i = 0; i < n; i++) {
      for (int j = 1; j < n; j++) {
        graph.add_edge(i, j);
      }
    }
    srand(0);
    for (int i = 0; i < n; i++) {
      graph.vertices[i].active = (rand() % 2);
    }

    return graph;
  }

  static  AdjacencyList BuildNCycle(int n) {
    //Cycle with N nodes
    AdjacencyList graph(n);
    for (int i = 0; i < n; i++) {
      if (i == n - 1)
        graph.add_edge(i, 0);
      else
        graph.add_edge(i, i + 1);
    }
    return graph;
  }

  //Fully connected MCS
  TEST_METHOD(N50FullyLexBFS) {
    AdjacencyList graph = BuildNFully(50);
    LexBFS lex(graph.num_vertices);
    lex.order(graph);
    Assert::IsTrue(lex.is_chordal(graph));
  }

  TEST_METHOD(N100FullyLexBFS) {
    AdjacencyList graph = BuildNFully(100);
    LexBFS lex(graph.num_vertices);
    lex.order(graph);
    Assert::IsTrue(lex.is_chordal(graph));
  }

  TEST_METHOD(N200FullyLexBFS) {
    AdjacencyList graph = BuildNFully(200);
    LexBFS lex(graph.num_vertices);
    lex.order(graph);
    Assert::IsTrue(lex.is_chordal(graph));
  }

  TEST_METHOD(N500FullyLexBFS) {
    AdjacencyList graph = BuildNFully(500);
    LexBFS lex(graph.num_vertices);
    lex.order(graph);
    Assert::IsTrue(lex.is_chordal(graph));
  }

  TEST_METHOD(N1000FullyLexBFS) {
    AdjacencyList graph = BuildNFully(1000);
    LexBFS lex(graph.num_vertices);
    lex.order(graph);
    Assert::IsTrue(lex.is_chordal(graph));
  }

  //Fully connected LexBFS
  TEST_METHOD(N50FullyMCS) {
    AdjacencyList graph = BuildNFully(50);
    map<int, int>order = MCS::order(graph);
    Assert::IsTrue(MCS::is_chordal(graph, order));
  }

  TEST_METHOD(N100FullyMCS) {
    AdjacencyList graph = BuildNFully(100);
    map<int, int>order = MCS::order(graph);
    Assert::IsTrue(MCS::is_chordal(graph, order));
  }

  TEST_METHOD(N200FullyMCS) {
    AdjacencyList graph = BuildNFully(200);
    map<int, int>order = MCS::order(graph);
    Assert::IsTrue(MCS::is_chordal(graph, order));
  }

  TEST_METHOD(N500FullyMCS) {
    AdjacencyList graph = BuildNFully(500);
    map<int, int>order = MCS::order(graph);
    Assert::IsTrue(MCS::is_chordal(graph, order));
  }

  TEST_METHOD(N1000FullyMCS) {
    AdjacencyList graph = BuildNFully(1000);
    map<int, int>order = MCS::order(graph);
    Assert::IsTrue(MCS::is_chordal(graph, order));
  }

  //Cycle MCS
  TEST_METHOD(N50CycleLexBFS) {
    AdjacencyList graph = BuildNCycle(50);
    LexBFS lex(graph.num_vertices);
    lex.order(graph);
    Assert::IsFalse(lex.is_chordal(graph));
  }

  TEST_METHOD(N100CycleLexBFS) {
    AdjacencyList graph = BuildNCycle(100);
    LexBFS lex(graph.num_vertices);
    lex.order(graph);
    Assert::IsFalse(lex.is_chordal(graph));
  }

  TEST_METHOD(N200CycleLexBFS) {
    AdjacencyList graph = BuildNCycle(200);
    LexBFS lex(graph.num_vertices);
    lex.order(graph);
    Assert::IsFalse(lex.is_chordal(graph));
  }

  TEST_METHOD(N500CycleLexBFS) {
    AdjacencyList graph = BuildNCycle(500);
    LexBFS lex(graph.num_vertices);
    lex.order(graph);
    Assert::IsFalse(lex.is_chordal(graph));
  }

  TEST_METHOD(N1000CycleLexBFS) {
    AdjacencyList graph = BuildNCycle(1000);
    LexBFS lex(graph.num_vertices);
    lex.order(graph);
    Assert::IsFalse(lex.is_chordal(graph));
  }

  TEST_METHOD(N10000CycleLexBFS) {
    AdjacencyList graph = BuildNCycle(10000);
    LexBFS lex(graph.num_vertices);
    lex.order(graph);
    Assert::IsFalse(lex.is_chordal(graph));
  }

  //Cycle LexBFS
  TEST_METHOD(N50CycleMCS) {
    AdjacencyList graph = BuildNCycle(50);
    map<int, int>order = MCS::order(graph);
    Assert::IsFalse(MCS::is_chordal(graph, order));
  }

  TEST_METHOD(N100CycleMCS) {
    AdjacencyList graph = BuildNCycle(100);
    map<int, int>order = MCS::order(graph);
    Assert::IsFalse(MCS::is_chordal(graph, order));
  }

  TEST_METHOD(N200CycleMCS) {
    AdjacencyList graph = BuildNCycle(200);
    map<int, int>order = MCS::order(graph);
    Assert::IsFalse(MCS::is_chordal(graph, order));
  }

  TEST_METHOD(N500CycleMCS) {
    AdjacencyList graph = BuildNCycle(500);
    map<int, int>order = MCS::order(graph);
    Assert::IsFalse(MCS::is_chordal(graph, order));
  }

  TEST_METHOD(N1000CycleMCS) {
    AdjacencyList graph = BuildNCycle(1000);
    map<int, int>order = MCS::order(graph);
    Assert::IsFalse(MCS::is_chordal(graph, order));
  }

  TEST_METHOD(N10000CycleMCS) {
    AdjacencyList graph = BuildNCycle(10000);
    map<int, int>order = MCS::order(graph);
    Assert::IsFalse(MCS::is_chordal(graph, order));
  }


  //FullyHalfDeleted connected MCS
  TEST_METHOD(N500FullyHalfDeletedLexBFS) {
    AdjacencyList graph = BuildNFullyHalfDeleted(500);
    LexBFS lex(graph.num_vertices);
    lex.order(graph);
    Assert::IsTrue(lex.is_chordal(graph));
  }

  TEST_METHOD(N1000FullyHalfDeletedLexBFS) {
    AdjacencyList graph = BuildNFullyHalfDeleted(1000);
    LexBFS lex(graph.num_vertices);
    lex.order(graph);
    Assert::IsTrue(lex.is_chordal(graph));
  }

  TEST_METHOD(N2000FullyHalfDeletedLexBFS) {
    AdjacencyList graph = BuildNFullyHalfDeleted(2000);
    LexBFS lex(graph.num_vertices);
    lex.order(graph);
    Assert::IsTrue(lex.is_chordal(graph));
  }

  //FullyHalfDeleted connected LexBFS
  TEST_METHOD(N500FullyHalfDeletedMCS) {
    AdjacencyList graph = BuildNFullyHalfDeleted(500);
    map<int, int>order = MCS::order(graph);
    Assert::IsTrue(MCS::is_chordal(graph, order));
  }

  TEST_METHOD(N1000FullyHalfDeletedMCS) {
    AdjacencyList graph = BuildNFullyHalfDeleted(1000);
    map<int, int>order = MCS::order(graph);
    Assert::IsTrue(MCS::is_chordal(graph, order));
  }

  TEST_METHOD(N2000FullyHalfDeletedMCS) {
    AdjacencyList graph = BuildNFullyHalfDeleted(2000);
    map<int, int>order = MCS::order(graph);
    Assert::IsTrue(MCS::is_chordal(graph, order));
  }
  };
}