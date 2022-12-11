#include <iostream>
#include <vector>       // std::vector
#include <random> 
#include <memory>
#include <stack>
#include <list>

#include "graph.hpp"
#include "dijkstra.hpp"
#include "min_cost_max_flow.hpp"
#include "bruteforce.hpp"

using namespace std;

void solve_even_graph(Graph g){
    Graph symetric_g = set_up_Symetric_graph(g);
    
    if (!isSymetric(symetric_g)){
        cout << "graph sould have been symetric but isnt\n";
        symetric_g.printOut();
        throw "error";
    }
    
    int tour_cost;
    vector<int> Euler_path = Euler_in_directed_from_vertex(0, symetric_g, &tour_cost);
    
}