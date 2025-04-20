#ifndef MST_H
#define MST_H

#include "Graph.h"

#include <iostream>
#include <limits>
#include <unordered_map>
#include <numeric>

void generate_mst_prim(Graph& g); // Task 7
int compute_mst_cost(Graph& g);

int find_set(std::vector<int>& parent, int v);
void generate_mst_kruskal(Graph& g); // Task 8


#endif
