#ifndef _DIJKSTRA_HPP_
#define _DIJKSTRA_HPP_

#include <vector>
// #include <vector>

#include "graph.hpp"


std::vector<int> dijkstra_for_3arc_sets(int start_V, int target_V, Graph g, std::vector<std::vector<int>> A1, std::vector<std::vector<int>> A2,
                                            std::vector<std::vector<int>> A3, std::vector<std::vector<int>> A1_2_cost);
int dijkstra_mixed_graph(int start_V, int target_V, Graph g);


#endif 
