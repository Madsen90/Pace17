#include "stdafx.h"
#include "CppUnitTest.h"
#include "../CPPace.Lib/LexBFS.h"
#include "../CPPace.Lib/Kernelizer.h"
#include "../CPPace.Lib/GraphGenerator.h"
#include <iostream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace CPPaceTests {
  TEST_CLASS(MCSLexBFSComparison) {
  public:
    //Fully connected MCS
    TEST_METHOD(N50FullyMCS) {
      AdjacencyList graph = GraphGenerator::fully_connected(50);
      LexBFS lex(graph.num_vertices);
      lex.order(graph);
      Assert::IsTrue(lex.is_chordal(graph));
    }

    TEST_METHOD(N100FullyMCS) {
      AdjacencyList graph = GraphGenerator::fully_connected(100);
      LexBFS lex(graph.num_vertices);
      lex.order(graph);
      Assert::IsTrue(lex.is_chordal(graph));
    }
  
    TEST_METHOD(N200FullyMCS) {
      AdjacencyList graph = GraphGenerator::fully_connected(200);
      LexBFS lex(graph.num_vertices);
      lex.order(graph);
      Assert::IsTrue(lex.is_chordal(graph));
    }

    TEST_METHOD(N500FullyMCS) {
      AdjacencyList graph = GraphGenerator::fully_connected(500);
      LexBFS lex(graph.num_vertices);
      lex.order(graph);
      Assert::IsTrue(lex.is_chordal(graph));
    }

    TEST_METHOD(N1000FullyMCS) {
      AdjacencyList graph = GraphGenerator::fully_connected(1000);
      LexBFS lex(graph.num_vertices);
      lex.order(graph);
      Assert::IsTrue(lex.is_chordal(graph));
    }

    //Fully connected LexBFS
    TEST_METHOD(N50FullyLexBFS) {
      AdjacencyList graph = GraphGenerator::fully_connected(50);
      map<int, int>order = Kernelizer::MCS(graph);
      Assert::IsTrue(Kernelizer::is_chordal_given_order(graph, order));
    }

    TEST_METHOD(N100FullyLexBFS) {
      AdjacencyList graph = GraphGenerator::fully_connected(100);
      map<int, int>order = Kernelizer::MCS(graph);
      Assert::IsTrue(Kernelizer::is_chordal_given_order(graph, order));
    }
  
    TEST_METHOD(N200FullyLexBFS) {
      AdjacencyList graph = GraphGenerator::fully_connected(200);
      map<int, int>order = Kernelizer::MCS(graph);
      Assert::IsTrue(Kernelizer::is_chordal_given_order(graph, order));
    }

    TEST_METHOD(N500FullyLexBFS) {
      AdjacencyList graph = GraphGenerator::fully_connected(500);
      map<int, int>order = Kernelizer::MCS(graph);
      Assert::IsTrue(Kernelizer::is_chordal_given_order(graph, order));
    }

    TEST_METHOD(N1000FullyLexBFS) {
      AdjacencyList graph = GraphGenerator::fully_connected(1000);
      map<int, int>order = Kernelizer::MCS(graph);
      Assert::IsTrue(Kernelizer::is_chordal_given_order(graph, order));
    }

    //Cycle MCS
    TEST_METHOD(N50CycleMCS) {
      AdjacencyList graph = GraphGenerator::cycle(50);
      LexBFS lex(graph.num_vertices);
      lex.order(graph);
      Assert::IsFalse(lex.is_chordal(graph));
    }

    TEST_METHOD(N100CycleMCS) {
      AdjacencyList graph = GraphGenerator::cycle(100);
      LexBFS lex(graph.num_vertices);
      lex.order(graph);
      Assert::IsFalse(lex.is_chordal(graph));
    }

    TEST_METHOD(N200CycleMCS) {
      AdjacencyList graph = GraphGenerator::cycle(200);
      LexBFS lex(graph.num_vertices);
      lex.order(graph);
      Assert::IsFalse(lex.is_chordal(graph));
    }

    TEST_METHOD(N500CycleMCS) {
      AdjacencyList graph = GraphGenerator::cycle(500);
      LexBFS lex(graph.num_vertices);
      lex.order(graph);
      Assert::IsFalse(lex.is_chordal(graph));
    }

    TEST_METHOD(N1000CycleMCS) {
      AdjacencyList graph = GraphGenerator::cycle(1000);
      LexBFS lex(graph.num_vertices);
      lex.order(graph);
      Assert::IsFalse(lex.is_chordal(graph));
    }

    TEST_METHOD(N10000CycleMCS) {
      AdjacencyList graph = GraphGenerator::cycle(10000);
      LexBFS lex(graph.num_vertices);
      lex.order(graph);
      Assert::IsFalse(lex.is_chordal(graph));
    }

    //Cycle LexBFS
    TEST_METHOD(N50CycleLexBFS) {
      AdjacencyList graph = GraphGenerator::cycle(50);
      map<int, int>order = Kernelizer::MCS(graph);
      Assert::IsFalse(Kernelizer::is_chordal_given_order(graph, order));
    }

    TEST_METHOD(N100CycleLexBFS) {
      AdjacencyList graph = GraphGenerator::cycle(100);
      map<int, int>order = Kernelizer::MCS(graph);
      Assert::IsFalse(Kernelizer::is_chordal_given_order(graph, order));
    }

    TEST_METHOD(N200CycleLexBFS) {
      AdjacencyList graph = GraphGenerator::cycle(200);
      map<int, int>order = Kernelizer::MCS(graph);
      Assert::IsFalse(Kernelizer::is_chordal_given_order(graph, order));
    }

    TEST_METHOD(N500CycleLexBFS) {
      AdjacencyList graph = GraphGenerator::cycle(500);
      map<int, int>order = Kernelizer::MCS(graph);
      Assert::IsFalse(Kernelizer::is_chordal_given_order(graph, order));
    }

    TEST_METHOD(N1000CycleLexBFS) {
      AdjacencyList graph = GraphGenerator::cycle(1000);
      map<int, int>order = Kernelizer::MCS(graph);
      Assert::IsFalse(Kernelizer::is_chordal_given_order(graph, order));
    }
  
    TEST_METHOD(N10000CycleLexBFS) {
      AdjacencyList graph = GraphGenerator::cycle(10000);
      map<int, int>order = Kernelizer::MCS(graph);
      Assert::IsFalse(Kernelizer::is_chordal_given_order(graph, order));
    }


    //FullyHalfDeleted connected MCS
    TEST_METHOD(N500FullyHalfDeletedMCS) {
      AdjacencyList graph = GraphGenerator::half_connected(500);
      LexBFS lex(graph.num_vertices);
      lex.order(graph);
      Assert::IsTrue(lex.is_chordal(graph));
    }

    TEST_METHOD(N1000FullyHalfDeletedMCS) {
      AdjacencyList graph = GraphGenerator::half_connected(1000);
      LexBFS lex(graph.num_vertices);
      lex.order(graph);
      Assert::IsTrue(lex.is_chordal(graph));
    }

    TEST_METHOD(N2000FullyHalfDeletedMCS) {
      AdjacencyList graph = GraphGenerator::half_connected(2000);
      LexBFS lex(graph.num_vertices);
      lex.order(graph);
      Assert::IsTrue(lex.is_chordal(graph));
    }

    //FullyHalfDeleted connected LexBFS
    TEST_METHOD(N500FullyHalfDeletedLexBFS) {
      AdjacencyList graph = GraphGenerator::half_connected(500);
      map<int, int>order = Kernelizer::MCS(graph);
      Assert::IsTrue(Kernelizer::is_chordal_given_order(graph, order));
    }

    TEST_METHOD(N1000FullyHalfDeletedLexBFS) {
      AdjacencyList graph = GraphGenerator::half_connected(1000);
      map<int, int>order = Kernelizer::MCS(graph);
      Assert::IsTrue(Kernelizer::is_chordal_given_order(graph, order));
    }

    TEST_METHOD(N2000FullyHalfDeletedLexBFS) {
      AdjacencyList graph = GraphGenerator::half_connected(2000);
      map<int, int>order = Kernelizer::MCS(graph);
      Assert::IsTrue(Kernelizer::is_chordal_given_order(graph, order));
    }
  };
}
