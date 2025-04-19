#include "Graph.h"
#include "MinHeap.h"
#include "MinHeap.cpp"
#include "Queue.h"

#include <unistd.h>
#include <climits>
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
            std::cout << edges[i][j].distance << " miles, $" << edges[i][j].cost << "} ";
        }
        std::cout << " }\n";
    }
}

void Graph::BFS(Vertex& ver) {
    clean_visited();

    int i = get_vertex_index(ver);
    if (i == -1) {
        throw std::string("BFS: Vertex is not in the graph");
    }
    Queue<int> q;
    q.enQueue(i);
    vertices[i].setVisited(true);

    while (!q.empty()) {
        int i = q.getFront();
        std::cout << vertices[i].getName() << ' ';
        for(int j = 0; j < edges[i].size(); j++) {
            int adjacent_ver = edges[i][j].dest;
            if (vertices[adjacent_ver].getVisited() == false) {
                vertices[adjacent_ver].setVisited(true);
                q.enQueue(adjacent_ver);
            }
        }
        q.deQueue();
    }

    clean_visited();
}


void Graph::clean_visited() {
    for(Vertex& v : vertices) {
        v.setVisited(false);
    }
}

int Graph::dijkstra_shortest_path(const Vertex& src, const Vertex& dest, std::vector<Vertex>& path) {
	int i_src = get_vertex_index(src);
	int i_dest = get_vertex_index(dest);

	if (i_src == -1 || i_dest == -1) {
		throw std::string("Shortest path: incorrect vertices");
	}

	clean_visited();
	std::vector<int> distances(vertices.size(), intMAX); // represents the distances from source to all other vertices
	std::vector<int> predecessors(vertices.size(), -1); // tracks the predecessors for path reconstruction

	// set initial distances
	distances[i_src] = 0;

	MinHeap<Edge> heap;
	heap.insert(Edge(i_src, i_src, 0, 0)); // Insert source vertex with distance 0
	int vertices_visited = 0;

	while (vertices_visited < vertices.size()) {
		// Get the edge with the smallest distance
		Edge e = heap.delete_min();
		int cur_ver = e.dest;

		// Mark the current vertex as visited
		vertices[cur_ver].setVisited(true);
		vertices_visited++;

		// If we reached the destination, stop
		if (cur_ver == i_dest) break;

		// Explore neighbors
		for (const Edge& edge : edges[cur_ver]) {
			int adj = edge.dest;
			if (!vertices[adj].getVisited()) {
				int new_dist = distances[cur_ver] + edge.distance;
				if (new_dist < distances[adj]) {
					distances[adj] = new_dist;
					predecessors[adj] = cur_ver; // Store the predecessor
					heap.insert(edge);
				}
			}
		}
	}

	// Reconstruct the path from destination to source using the predecessors
	int cur_ver = i_dest;
	while (cur_ver != -1) {
		path.push_back(vertices[cur_ver]);
		cur_ver = predecessors[cur_ver];
	}

	std::reverse(path.begin(), path.end()); // Reverse to get the correct order (source to destination)

	return distances[i_dest];
}

const std::vector<Edge>& Graph::get_edges(int vertex_idx) const {
	return edges[vertex_idx];
}

Vertex* Graph::find_vertex_by_name(const std::string& name) {
    for (Vertex& v : vertices) {
        if (v.getName() == name) {
            return &v;
        }
    }
    return nullptr;
}


void Graph::find_paths_to_town(const std::string& origin_airport, const std::string& dest_town) {
    bool paths_found = false;

    // Get the origin airport vertex pointer
    Vertex* origin = find_vertex_by_name(origin_airport);
    if (origin == nullptr) {
        std::cout << "Origin airport not found. Please check spelling and try again.\n";
        return;
    }

    // Look for any paths to airports in the destination town
    for (const Vertex& v : vertices) {
        if (v.getTown() == dest_town && v.getName() != origin_airport) {
            std::vector<Vertex> path;
            int distance = dijkstra_shortest_path(*origin, v, path);

            if (distance != intMAX) {
                // Print each path found from origin to town
                std::cout << "Path from " << origin_airport << " to " << v.getName() << ": ";

                for (size_t i = 0; i < path.size(); i++) {
                    std::cout << path[i].getName();
                    if (i != path.size() - 1) {
                        std::cout << " -> ";
                    }
                }

                std::cout << "\nThe length is " << distance << "\n";

                int total_cost = 0;
                for (size_t i = 0; i < path.size() - 1; i++) {
                    int cur_idx = get_vertex_index(path[i]);
                    for (const Edge& edge : get_edges(cur_idx)) {
                        if (edge.dest == get_vertex_index(path[i + 1])) {
                            total_cost += edge.cost;
                            break;
                        }
                    }
                }
                std::cout << "The cost is " << total_cost << ".\n";
                paths_found = true;
            }
        }
    }

    if (!paths_found) {
        std::cout << "No paths found to the destination town.\n";
    }
}

void Graph::count_and_display_connections() const {
    int n = vertices.size();
    // Array of connection counts
    std::vector<int> counts(n, 0);

    // Since edges are stored both ways, edges[i].size() gives total direct connections
    for (int i = 0; i < n; ++i) {
        counts[i] = edges[i].size();
    }

    // Create an index array for sorting
    std::vector<int> idx(n);
    for (int i = 0; i < n; ++i) idx[i] = i;

    // Simple selection sort on idx by counts descending
    for (int i = 0; i < n - 1; ++i) {
        int maxPos = i;
        for (int j = i + 1; j < n; ++j) {
            if (counts[idx[j]] > counts[idx[maxPos]]) {
                maxPos = j;
            }
        }
        if (maxPos != i) {
            int tmp = idx[i]; idx[i] = idx[maxPos]; idx[maxPos] = tmp;
        }
    }

    // Display results
    std::cout << "\nAirports sorted by total direct flight connections:\n";
    for (int i = 0; i < n; ++i) {
        int v_idx = idx[i];
        std::cout << vertices[v_idx].getName() << ": " << counts[v_idx] << " connections" << std::endl;
    }
}

Graph Graph::create_undirected_graph() const {
    Graph gu;
    int n = vertices.size();

    // Insert all vertices
    for (const Vertex& v : vertices) {
        gu.insert_vertex(v);
    }

    // For each directed edge, consider pair (i,j) once
    for (int i = 0; i < n; ++i) {
        for (const Edge& e : edges[i]) {
            int j = e.dest;
            if (i < j) {
                // Check for reverse edge
                int cost = e.cost;
                for (const Edge& rev : edges[j]) {
                    if (rev.dest == i) {
                        cost = std::min(cost, rev.cost);
                        break;
                    }
                }
                // Add as undirected: distance ignored (0), cost as weight
                gu.add_edge(vertices[i], vertices[j], 0, cost);
            }
        }
    }
    return gu;
}