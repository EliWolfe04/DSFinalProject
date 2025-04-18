#include "Graph.h"
#include "MinHeap.h"
#include "MinHeap.cpp"

#include <queue>
#include <unistd.h> 
#include <iostream>
#define intMAX 3000


void Graph::insert_vertex(const Vertex& ver) {
    if (get_vertex_index(ver) == -1) {
        vertices.push_back(ver); //insert the vertex to the array of vertices
        std::vector<Edge> tmp;
        edges.push_back(tmp); //insert empty vector to the edges
    }
}

int Graph::get_vertex_index(const Vertex& ver) {
    for(int i = 0; i < vertices.size(); i++) {
        if (vertices[i].getName() == ver.getName()) {
            return i;
        }
    }

    return -1;
}

void Graph::add_edge(const Vertex& ver1, const Vertex& ver2, int distance, int cost) {
    int i1 = get_vertex_index(ver1);
    int i2 = get_vertex_index(ver2);
    if (i1 == -1 || i2 == -1) {
        throw std::string("Add_edge: incorrect vertices");
    }
    Edge v(i1, i2, distance, cost);
    Edge v2(i2, i1, distance, cost);
    edges[i1].push_back(v);
    if (i1 != i2) {
        edges[i2].push_back(v2);
    }
}


void Graph::print() const {
    for (int i = 0; i < vertices.size(); i++) {
        std::cout << "{ " << vertices[i].getName() << ": ";
        for(int j = 0; j < edges[i].size(); j++) {
            std::cout << '{' << vertices[edges[i][j].dest].getName() << ", ";
            std::cout << edges[i][j].distance << ", $" << edges[i][j].cost << "} ";
        }
        std::cout << " }\n";
    }
}

void print_queue(std::queue<Vertex> q) {
  while (!q.empty()) {
    std::cout << q.front().getName() << " ";
    q.pop();
  }
  std::cout << std::endl;
}


void Graph::BFS(Vertex& ver) {
    clean_visited();

    int i = get_vertex_index(ver);
    if (i == -1) {
        throw std::string("BFS: Vertex is not in the graph");
    }
    std::queue<int> q;
    q.push(i);
    vertices[i].setVisited(true);

    while (!q.empty()) {
        int i = q.front();
        std::cout << vertices[i].getName() << ' ';
        for(int j = 0; j < edges[i].size(); j++) {
            int adjacent_ver = edges[i][j].dest;
            if (vertices[adjacent_ver].getVisited() == false) {
                vertices[adjacent_ver].setVisited(true);
                q.push(adjacent_ver);
            }
        }
        q.pop();
    }

    clean_visited();
}


void Graph::clean_visited() {
    for(Vertex& v : vertices) {
        v.setVisited(false);
    }
}

int Graph::dijkstra_shortest_path(const Vertex& src, const Vertex& dest) {
    int i_src = get_vertex_index(src);
    int i_dest = get_vertex_index(dest);

    if (i_src == -1 || i_dest == -1) {
        throw std::string("Shortest path: incorrect vertices"); 
    }

    clean_visited();
    std::vector<int> distances(vertices.size()); //represents the distances from source to all other vertices

    //set initial distances
    for(int i = 0; i < distances.size(); i++) {
        distances[i] = (i == i_src) ? 0 : intMAX;
    }

    MinHeap<Edge> heap;
    int vertices_visited = 0;
    int cur_ver = i_src;

    // Continue until all vertices have been visited
    while (vertices_visited < vertices.size()) {
        // Set current vertex to cur_ver (current vertex being processed)
        int i = cur_ver;
    
        // Iterate through all neighbors (edges) of the current vertex
        for (int j = 0; j < edges[i].size(); j++) {
            // Get the destination vertex of the current edge
            int i_adjacent_ver = edges[i][j].dest;
    
            // Check if the adjacent vertex has not been visited yet
            if (vertices[i_adjacent_ver].getVisited() == false) {
                // Insert the edge into the min-heap (for processing later in Dijkstra's algorithm)
                heap.insert(edges[i][j]);
    
                // Calculate the new distance from the source to the adjacent vertex through the current vertex
                int dist_from_source = distances[i] + edges[i][j].distance;
    
                // If this new distance is shorter than the previously known distance to the adjacent vertex, update it
                if (dist_from_source < distances[i_adjacent_ver]) {
                    // Update the shortest distance to the adjacent vertex
                    distances[i_adjacent_ver] = dist_from_source;
    
                    // Optional debugging: print all the distances at each step to track the changes
                    for (int i : distances) {
                        std::cout << i << ' ';
                    }
                    std::cout << '\n';
                }
            }
        }
    
        // After exploring all neighbors, get the edge with the smallest weight from the heap
        Edge e = heap.delete_min();
    
        // Update the current vertex to the destination of the edge with the smallest weight
        cur_ver = e.dest;
    
        // Mark the current vertex as visited
        vertices[i].setVisited(true);
    
        // Increment the count of visited vertices
        vertices_visited++;
    }


    clean_visited();

    return distances[i_dest];
}