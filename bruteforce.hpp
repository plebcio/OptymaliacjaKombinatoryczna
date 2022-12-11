#ifndef _BRUTEFORCE_H_
#define _BRUTEFORCE_H_

#include <vector>
#include <list>

#include "graph.hpp"

bool isEven(Graph g);
bool isSymetric(Graph g);
int DFS(int v, bool visited[], Graph g);
bool isConnected_from_v(int vertex, const Graph &g);
int cost_DFS(int curr_V, int target_V, int curr_cost, std::vector <bool> visited, Graph g);
Graph make_graph_even(Graph g);
int find_best_route(Graph g);
Graph make_even_brute_force(Graph g);

#endif