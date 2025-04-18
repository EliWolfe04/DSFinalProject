#include <iostream>
#include <bits/stdc++.h>

#include "Graph.h"
#include "Ui.h"
#include "Vertex.h"


// useful tool for strings that replaces all instances of a character with another
std::string replaceAll(std::string str, const std::string& from, const std::string& to) {
    
    int startPos = 0;
    while((startPos = str.find(from, startPos)) != std::string::npos) {
        str.replace(startPos, from.length(), to);
        startPos += to.length();
    }
    
    return str;
}

int main() {
    
    std::ifstream f("airports.txt"); // read airport list, .csv does not work in onlinegdb for whatever reason ¯\_(ツ)_/¯
    
    if(!f.is_open()) return 1; // if not open don't do things (it should be open)
    
    std::string line; // current line we're reading
    getline(f,line); // reads the first line of the csv which is the header columns
    
    Graph airports;
    
    while(getline(f,line)) { // loop through all of the lines in the file
        std::vector<std::string> v; // create a vector to hold all the data of each line
        std::stringstream lineStr(replaceAll(line, ", ", " ")); // replaces the stupid comma in the town names
        while(getline(lineStr, line, ',')) v.push_back(line); // puts each section of data in the vector
        
        // Creates the vertex points, \" is an escape character for the quotation character
        Vertex v1(v[0], replaceAll(v[2], "\"", ""));
        Vertex v2(v[1], replaceAll(v[3], "\"", ""));
        
        // Vertexes go into the airport graph
        airports.insert_vertex(v1);
        airports.insert_vertex(v2);
        
        airports.add_edge(v1, v2, std::stoi(v[4]), std::stoi(v[5])); // Add the edge with the distance and cost put in
    }
    
display_menu(airports);


    return 0;
}

