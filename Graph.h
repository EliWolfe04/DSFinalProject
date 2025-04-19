#ifndef GRAPH_H
#define GRAPH_H
#include "Vertex.h"
#include "Edge.h"
#include <algorithm>
#include <vector>

class Graph {
public:
    Graph() {}

    void insert_vertex(const Vertex& ver);
    void add_edge(const Vertex& ver1, const Vertex& ver2, int distance, int cost); //connect ver1 with ver2

    void print() const;

    void DFS(Vertex& ver);
    void BFS(Vertex& ver);
    int dijkstra_shortest_path(const Vertex& src, const Vertex& dest, std::vector<Vertex>& path);
    void find_paths_to_town(const std::string& origin_airport, const std::string& dest_town);


    Vertex* find_vertex_by_name(const std::string& name);
    const std::vector<Vertex>& getVertices() const {
    return vertices;
}

    const std::vector<Edge>& get_edges(int vertex_idx) const;
    int get_vertex_index(const Vertex& ver);
    void count_and_display_connections() const;
    Graph create_undirected_graph() const;

    
private:
    std::vector<Vertex> vertices; //nodes
    std::vector<std::vector<Edge>> edges; //connections

    void clean_visited();

    void DFS_helper(Vertex& ver);

};

#endif
