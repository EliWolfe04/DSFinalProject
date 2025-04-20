#include "Ui.h"
#include "Edge.h"
#include <iostream>
#include <string>
#include <climits>
#include <limits>
#include <vector>
#include <iomanip>
#include "Queue.h"

#define intMAX 3000 

void display_menu(Graph& g) {
    int choice = 0;

    while (true) {
        std::cout << "\n--- Welcome to Florida Poly Airport Optimization System ---\n";
        std::cout << "1. Find shortest path between two airports\n";
        std::cout << "2. Find all shortest paths between an airport and a state.\n";
        std::cout << "3. Find the shortest path between airports given a number of stops\n";
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
            find_shortest_path_ui(g); // Task 2
            break;
        case 2:
            find_shortest_path_airport_state(g); // Task 3
            break;
        case 3:
            find_shortest_path_with_k_stops(g); // Task 4
            break;
        case 5:
            g.count_and_display_connections(); // Task 5
            break;
        case 6: {
            Graph gu = g.create_undirected_graph(); // Task 6
            std::cout << "\n--- Undirected Graph G_u ---\n";
            gu.print();
            break;
        }
        case 0:
            std::cout << "Exiting...\n";
            return;
        case 4: case 7:
            std::cout << "Sorry, that option isn't implemented yet.\n";
            break;
        default:
            std::cout << "Invalid choice. Try again.\n";
            break;
        }
    }
}

// Task 3
void find_shortest_path_airport_state(Graph& g) {
    std::string airport_name, state;

    std::cout << "Enter the name of the starting aiport: ";
    getline(std::cin, airport_name);

    std::cout << "Enter the abbreviation of the state: ";
    getline(std::cin, state);

    Vertex airport;
    std::vector<Vertex> v;

    // Loop through all of the vertices in the graph to find the origin airport and all of the airports in the state
    for (Vertex ver : g.getVertices()) {
        if (ver.getName() == airport_name) {
            airport = ver;
            continue;
        }

        // Looks tries to find the state abbreviation in each vertice's town name to add to the list
        int found = ver.getTown().find(state);
        if (found != std::string::npos) {
            v.push_back(ver);
            continue;
        }
    }

    // Checks to see if the airport vertices was found
    if (airport.getName() == "") {
        std::cout << "No airports found by the name " << airport_name << std::endl;
        return;
    }

    // Checks to see if any airports in the state were found
    if (v.empty()) {
        std::cout << "No airports found in " << state << std::endl;
        return;
    }

    std::cout << "Shortest paths from " << airport.getName() << " to " << state << " state airports are:" << std::endl << std::endl;
    std::cout << "Path " << std::setw(30) << "Length " << std::setw(10) << "Cost" << std::endl;

    for (Vertex ver : v) {
        std::vector<Vertex> path;
        int distance = g.dijkstra_shortest_path(airport, ver, path);

        // if the distance is > 3000 we do not count it
        if (distance == intMAX) continue;

        std::string path_str = "";

        // Creates a string for the path from the origin airport to the destination in the state
        for (int i = 0; i < path.size(); i++) {
            path_str += path[i].getName();
            if (i != path.size() - 1) path_str += "->";
        }

        // Finds the cost of the path
        int cost = 0;
        for (int i = 0; i < path.size() - 1; i++) {
            int cur_index = g.get_vertex_index(path[i]);
            for (Edge edge : g.get_edges(cur_index)) {
                if (edge.dest == g.get_vertex_index(path[i + 1])) {
                    cost += edge.cost;
                    break;
                }
            }
        }

        // Outputs the path, distance, and cost in a neat table
        std::cout << path_str << std::setw(31 - path_str.length() + (distance / 1000)) << distance << std::setw(13 - (distance / 1000)) << cost << std::endl;
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
        }
        else {
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
    }
    catch (const std::string& e) {
        // Handle any errors thrown by the algorith
        std::cout << "Error: " << e << '\n';
    }
}

// Task 4
void find_shortest_path_with_k_stops(Graph& g) {
    std::string origin_name, destination_name;
    int k;

    std::cout << "Enter the name of the starting airport: ";
    std::getline(std::cin, origin_name);

    std::cout << "Enter the name of the destination airport: ";
    std::getline(std::cin, destination_name);

    std::cout << "Enter the number of stops (K): ";
    std::cin >> k;
    std::cin.ignore();  // Clear input buffer

    Vertex* origin = g.find_vertex_by_name(origin_name);
    Vertex* destination = g.find_vertex_by_name(destination_name); // Find both airports

    if (!origin) {
        std::cout << "No airport found by the name " << origin_name << std::endl;
        return;
    }
    if (!destination) {
        std::cout << "No airport found by the name " << destination_name << std::endl;
        return;
    }

    int origin_index = g.get_vertex_index(*origin);
    int dest_index = g.get_vertex_index(*destination);

    struct QueueNode {
        std::vector<Vertex> path;
        int current_index;
        int distance;
        int stops;
    }; // struct to reduce the need for multiple queues

    Queue<QueueNode> q;
    q.enQueue({ {*origin}, origin_index, 0, 0 });

    int min_distance = INT_MAX;
    std::vector<Vertex> best_path;

    while (!q.empty()) {
        QueueNode node = q.getFront();
        q.deQueue();

        if (node.stops > k) continue;

        if (node.current_index == dest_index && node.stops == k) {
            if (node.distance < min_distance) {
                min_distance = node.distance;
                best_path = node.path;
            } // save optimal path with conditions met
            continue;
        }

        for (const Edge& edge : g.get_edges(node.current_index)) {
            std::vector<Vertex> new_path = node.path;
            new_path.push_back(g.getVertices()[edge.dest]);
            q.enQueue({ new_path, edge.dest, node.distance + edge.distance, node.stops + 1 });
        } // BFS for k stops
    }

    if (best_path.empty()) {
        std::cout << "No path exists from " << origin->getName()
            << " to " << destination->getName()
            << " with exactly " << k << " stops." << std::endl;
    }
    else {
        std::cout << "Shortest path from " << origin->getName()
            << " to " << destination->getName()
            << " with exactly " << k << " stops:" << std::endl;
        for (size_t i = 0; i < best_path.size(); ++i) {
            std::cout << best_path[i].getName();
            if (i != best_path.size() - 1) std::cout << " -> ";
        }
        std::cout << " (Distance: " << min_distance << ")" << std::endl;
    }
}
