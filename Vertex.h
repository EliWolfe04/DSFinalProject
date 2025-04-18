#ifndef VERTEX_H
#define VERTEX_H

#include <iostream>

class Vertex {
public:
    Vertex(const std::string& name = "", const std::string& town = "") : name(name), town(town), visited(false) {};
    const std::string& getName() const {return name; }
    const std::string& getTown() const {return town; }
    bool getVisited() const {return visited; }
    void setVisited(bool v) { visited = v; }

private:
    std::string name;
    std::string town;
    bool visited;
};

#endif