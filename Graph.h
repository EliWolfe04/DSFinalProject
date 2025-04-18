#ifndef GRAPH_H
#define GRAPH_H
#include "Vertex.h"
#include "Edge.h"
#include <vector>

class Graph {
public:
    Graph() {}

    void insert_vertex(const Vertex& ver);
    void add_edge(const Vertex& ver1, const Vertex& ver2, int distance, int cost); //connect ver1 with ver2

    void print() const;

    void DFS(Vertex& ver);
    void BFS(Vertex& ver);
    int dijkstra_shortest_path(const Vertex& src, const Vertex& dest);

private:
    std::vector<Vertex> vertices; //nodes
    std::vector<std::vector<Edge>> edges; //connections

    void clean_visited();

    void DFS_helper(Vertex& ver);
    int get_vertex_index(const Vertex& ver);
};

#endif
