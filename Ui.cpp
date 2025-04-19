#include "Ui.h"
#include "Edge.h"
#include <iostream>
#include <string>
#include <climits>
#include <limits>


#define intMAX 3000 

void display_menu(Graph& g) {
    int choice = 0;

    while (true) {
        std::cout << "\n--- Welcome to Florida Poly Airport Optimization System ---\n";
        std::cout << "1. Find shortest path between two airports\n";
        std::cout << "2. Option 2 (Not implemented yet)\n";
        std::cout << "3. Option 3 (Not implemented yet)\n";
        std::cout << "4. Option 4 (Not implemented yet)\n";
        std::cout << "5. Count and display total direct flight connections\n";
        std::cout << "6. Create undirected graph G_u and display it\n";
        std::cout << "7. Option 7 (Not implemented yet)\n";
        std::cout << "0. Exit\n";
        std::cout << "Enter your choice: ";

        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (choice) {
            case 1:
                find_shortest_path_ui(g);
                break;
            case 5:
                g.count_and_display_connections();
                break;
            case 6: {
                Graph gu = g.create_undirected_graph();
                std::cout << "\n--- Undirected Graph G_u ---\n";
                gu.print();
                break;
            }
            case 0:
                std::cout << "Exiting...\n";
                return;
            case 2: case 3: case 4: case 7:
                std::cout << "Sorry, that option isn't implemented yet.\n";
                break;
            default:
                std::cout << "Invalid choice. Try again.\n";
                break;
        }
    }
}

void find_shortest_path_ui(Graph& g) {
    std::string src_name, dest_name;

    // Ask user for the source and destination airport names
    std::cout << "Enter the name of the source airport: ";
    std::getline(std::cin, src_name);

    std::cout << "Enter the name of the destination airport: ";
    std::getline(std::cin, dest_name);

    const Vertex* src_ptr = nullptr;
    const Vertex* dest_ptr = nullptr;

    // Loop through all the vertices to find matches for the entered names
    for (const Vertex& v : g.getVertices()) {
        if (v.getName() == src_name) {
            src_ptr = &v;
        }
        if (v.getName() == dest_name) {
            dest_ptr = &v;
        }
    }

    // If we couldn’t find one or both of the airports, let the user know
    if (src_ptr == nullptr || dest_ptr == nullptr) {
        std::cout << "One or both airport names not found.\n";
        return;
    }

    std::vector<Vertex> path;
    try {
        // Run Dijkstra's algorithm to get the shortest path and distance
        int distance = g.dijkstra_shortest_path(*src_ptr, *dest_ptr, path);

        if (distance == intMAX) {
            // No valid path was found
            std::cout << "Shortest route from " << src_ptr->getName() << " to " << dest_ptr->getName() << ": None\n";
        } else {
            // We found a path!
            std::cout << "Shortest route from " << src_ptr->getName() << " to " << dest_ptr->getName() << ": ";
            
            for (size_t i = 0; i < path.size(); i++) {
                std::cout << path[i].getName();
                if (i != path.size() - 1) {
                    std::cout << " -> ";
                }
            }

            std::cout << "\n The length is " << distance << "\n The cost is ";
            
            // Calculate total cost based on the edges between each vertex in the path
            int total_cost = 0;
            for (size_t i = 0; i < path.size() - 1; i++) {
                int cur_idx = g.get_vertex_index(path[i]);
                for (const Edge& edge : g.get_edges(cur_idx)) {
                    if (edge.dest == g.get_vertex_index(path[i + 1])) {
                        total_cost += edge.cost;
                        break;
                    }
                }
            }
            std::cout << total_cost << ".\n";
        }
    } catch (const std::string& e) {
        // Handle any errors thrown by the algorith
        std::cout << "Error: " << e << '\n';
    }
}

