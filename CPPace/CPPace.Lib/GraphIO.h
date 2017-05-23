#pragma once
#include "EXPORT.h"

#include <iostream>
#include <string>
#include <unordered_map>
#include "AdjacencyList.h"

using namespace std;

class DLLEXPORT GraphIO {
public:
  class GraphContext {
  public:
    AdjacencyList graph;
    vector<string> vertex_names;
    unordered_map<string, set<pair<int, int>>> edge_color_map;
    unordered_map<string, set<int>> vertex_color_map;

    GraphContext(AdjacencyList& graph, vector<string>& vertex_names, unordered_map<string, set<pair<int, int>>>& edge_color_map, unordered_map<string, set<int>> vertex_color_map)
      : graph(graph),
        vertex_names(vertex_names),
        edge_color_map(edge_color_map),
        vertex_color_map(vertex_color_map)
    { }

    GraphContext(AdjacencyList& graph)
      : graph(graph),
        vertex_names(),
        edge_color_map(),
        vertex_color_map()
    {
      for (int u = 0; u < graph.num_vertices; u++)
        vertex_names.emplace_back(to_string(u));
    }

    string vertex_name(int vertex) {
      return vertex_names[vertex];
    }

    // Creates a graph context only containing active vertices
    GraphContext squash() {
      int squashed_num_vertices = 0;
      for (Vertex u : graph.vertices)
        if (u.active) squashed_num_vertices++;

      AdjacencyList squashed_graph(squashed_num_vertices);
      vector<string> squashed_vertex_names(squashed_num_vertices);
      unordered_map<string, set<pair<int, int>>> squashed_edge_color_map;
      unordered_map<string, set<int>> squashed_vertex_color_map;

      vector<int> squashed_index(graph.num_vertices);
      for (int u = 0, squashed_u = 0; u < graph.num_vertices; u++) {
        if (!graph.vertices[u].active) continue;
        
        // Assign u a squashed index in the squashed graph
        squashed_index[u] = squashed_u++;

        // Squash edges
        for (int v : graph.edges(u)) {
          if (u > v) // Make sure u and v are set in squashed_index
            squashed_graph.add_edge(squashed_index[u], squashed_index[v]);
        }

        // Squash vertex names
        squashed_vertex_names[squashed_index[u]] = vertex_names[u];
      }

      // Squash edge color map
      for (pair<string, set<pair<int, int>>> colored_edges : edge_color_map) {
        string color = colored_edges.first;
        set<pair<int, int>> edges = colored_edges.second;

        set<pair<int, int>> squashed_edges;
        for (pair<int, int> edge : edges) {
          if (!graph.vertices[edge.first].active || !graph.vertices[edge.second].active)
            continue;
          squashed_edges.emplace(squashed_index[edge.first], squashed_index[edge.second]);
        }

        if (squashed_edges.empty()) continue;
        squashed_edge_color_map[color] = squashed_edges;
      }

      // Squash vertex color map
      for (pair<string, set<int>> colored_vertices : vertex_color_map) {
        string color = colored_vertices.first;
        set<int> vertices = colored_vertices.second;

        set<int> squashed_vertices;
        for (int u : vertices) {
          if (!graph.vertices[u].active)
            continue;
          squashed_vertices.emplace(squashed_index[u]);
        }

        if (squashed_vertices.empty()) continue;
        squashed_vertex_color_map[color] = squashed_vertices;
      }

      return GraphContext(
        squashed_graph,
        squashed_vertex_names,
        squashed_edge_color_map,
        squashed_vertex_color_map);
    }
  };

  // EdgeList    Every line describes an edge as "<u> <v>"
  // DOT         Follows the DOT-format
  enum Format { EdgeList, DOT };
  static GraphContext read_from_stream(istream& stream);
  static GraphContext read_from_path(string path);
  static void write_to_stream(GraphContext& graph, ostream& stream, Format format = Format::EdgeList);
  static void write_to_path(GraphContext& graph, string path, Format format = Format::EdgeList);
};
