#define intMAX 3000  

#include "MinHeap.cpp"  
#include "MST.h"        

// Function to compute the weight of the Minimum Spanning Tree using Prim's algorithm
int compute_mst_cost(Graph& graph) {
    int V = graph.get_vertices().size();
    if (V == 0) return 0;  // Return 0 if graph has no vertices

    std::vector<bool> visited(V, false);  
    MinHeap<Edge> min_heap;               // MinHeap to pick minimum weight edge
    int total_cost = 0;

    visited[0] = true;
    for (const Edge& edge : graph.get_edges(0)) {
        min_heap.insert(edge);  // Insert all edges of starting vertex into heap
    }

    int visited_count = 1;

    while (visited_count < V && !min_heap.empty()) {
        Edge min_edge = min_heap.delete_min();  // Get edge with minimum cost

        if (visited[min_edge.dest]) {
            continue;  // Skip if destination is already visited
        }

        visited[min_edge.dest] = true;
        total_cost += min_edge.cost; 
        visited_count++;

        // Insert all adjacent edges of newly visited vertex
        for (const Edge& edge : graph.get_edges(min_edge.dest)) {
            if (!visited[edge.dest]) {
                min_heap.insert(edge);
            }
        }
    }

    if (visited_count != V) {
        throw std::runtime_error("Graph is not connected, MST cannot be formed");
    }

    return total_cost;
}


int find_min_cost_vertex(const std::vector<bool>& visited, const std::vector<int>& cost) {
    int min_cost = intMAX;
    int min_index = -1;
    for (size_t i = 0; i < cost.size(); ++i) {
        if (!visited[i] && cost[i] < min_cost) {
            min_cost = cost[i];
            min_index = static_cast<int>(i);
        }
    }
    return min_index;
}

// Prim's algorithm
void generate_mst_prim(Graph& g) {
    const std::vector<Vertex>& vertices = g.getVertices();
    int n = static_cast<int>(vertices.size());
    if (n == 0) {
        std::cout << "Graph is empty.\n";
        return;
    }

    std::vector<bool> visited(n, false);
    std::vector<int> cost(n, intMAX);   // Track minimum edge cost to each vertex
    std::vector<int> parent(n, -1);     

    cost[0] = 0;  

    for (int count = 0; count < n - 1; ++count) {
        int u = find_min_cost_vertex(visited, cost);

        if (u == -1) {
            std::cout << "Graph is disconnected. MST cannot be formed.\n";
            return;
        }

        visited[u] = true;

        for (const Edge& edge : g.get_edges(u)) {
            int v = edge.dest;
            if (!visited[v] && edge.cost < cost[v]) {
                cost[v] = edge.cost;
                parent[v] = u;  // Update parent if a cheaper edge is found
            }
        }
    }

    // Output MST edges and total cost
    int total_cost = 0;
    std::cout << "\n--- Minimum Spanning Tree ---\n";
    for (int i = 1; i < n; ++i) {
        if (parent[i] != -1) {
            std::cout << vertices[parent[i]].getName()
                      << " --(" << cost[i] << ")--> "
                      << vertices[i].getName() << '\n';
            total_cost += cost[i];
        }
    }
    std::cout << "Total MST Cost: " << total_cost << '\n';
}

// Disjoint
int find_set(std::vector<int>& parent, int v) {
    if (v != parent[v]) parent[v] = find_set(parent, parent[v]);
    return parent[v];
}

// Union of two sets
void union_sets(std::vector<int>& parent, std::vector<int>& rank, int a, int b) {
    a = find_set(parent, a);
    b = find_set(parent, b);
    if (a != b) {
        if (rank[a] < rank[b]) std::swap(a, b);
        parent[b] = a;
        if (rank[a] == rank[b]) rank[a]++;
    }
}

// Kruskal's algorithm
void generate_mst_kruskal(Graph& g) {
    const auto& vertices = g.getVertices();
    int n = vertices.size();

    if (n == 0) {
        std::cout << "Graph is empty.\n";
        return;
    }

    std::vector<Edge> all_edges;
    // Collect all edges
    for (int u = 0; u < n; ++u) {
        for (const Edge& e : g.get_edges(u)) {
            if (u < e.dest) {
                all_edges.push_back(e);
            }
        }
    }

    // Sort edges by increasing cost
    std::sort(all_edges.begin(), all_edges.end(), [](const Edge& a, const Edge& b) {
        return a.cost < b.cost;
    });

    std::vector<int> parent(n);  
    std::vector<int> rank(n, 0);
    std::iota(parent.begin(), parent.end(), 0);  // Each vertex is its own parent

    std::vector<std::vector<Edge>> MST(n);  // Group edges by connected components
    int total_cost = 0;

    for (const Edge& edge : all_edges) {
        int u = edge.src;
        int v = edge.dest;

        int set_u = find_set(parent, u);
        int set_v = find_set(parent, v);

        if (set_u != set_v) {
            MST[set_u].push_back(edge);  // Add edge
            union_sets(parent, rank, u, v);  // Union the sets
            total_cost += edge.cost;
        }
    }

    // Re-group MST by their final root after all unions
    std::unordered_map<int, std::vector<Edge>> Tree_groups;
    for (int i = 0; i < n; ++i) {
        int root = find_set(parent, i);
        for (const Edge& e : MST[i]) {
            Tree_groups[root].push_back(e);
        }
    }

    // Output Minimum Spanning Tree
    std::cout << "\n--- Minimum Spanning Tree (Kruskal) ---\n";
    for (const auto& pair : Tree_groups) {
        int root = pair.first;
        const auto& edges = pair.second;
        std::cout << "\nComponent rooted at: " << vertices[root].getName() << "\n";
        for (const Edge& e : edges) {
            std::cout << vertices[e.src].getName() << " --(" << e.cost << ")--> " << vertices[e.dest].getName() << '\n';
        }
    }
    std::cout << "\nTotal Cost of MST: " << total_cost << "\n";
}
