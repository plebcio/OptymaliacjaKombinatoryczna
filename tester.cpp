#include <iostream>
#include <vector>       // std::vector
#include <random> 
#include <memory>
#include <unordered_map>
#include <stack>

#include "graph.hpp"
#include "bruteforce.hpp"
#include "dijkstra.hpp"
#include "min_cost_max_flow.hpp"

using namespace std;

int main(){

    Graph testG(7
    );
    testG.add_arc(0, 6);
    testG.add_arc(4, 2);
     
    testG.add_edge(1, 2);
    testG.add_edge(2, 3);
    testG.add_edge(3, 4);
    testG.add_edge(4, 0);

    testG.fill_cost_x(2);

    testG.printOut();

    testG.creat_prev_list();
    vector<bool> tmp(testG.n_vertex, false);

    std::cout << std::boolalpha <<
        // "is connected from 0 " << isConnected_from_v(0, testG) << endl <<
        // "is connected from 1 " << isConnected_from_v(1, testG) << endl << 
        // "is connected from 2 " << isConnected_from_v(2, testG) << endl <<
        // "is connected from 3 " << isConnected_from_v(3, testG) << endl << 
        // "is connected from 4 " << isConnected_from_v(4, testG) << endl <<
        endl <<
        "is even " << isEven(testG) << endl << 
        endl <<
        "is symetric " << isSymetric(testG) << endl << 
        endl <<
        // "cost from 0 -> 2 = " << dijkstra_mixed_graph(4 ,3, testG) <<
        endl;

    Graph new_graph = make_even_brute_force(testG);

    new_graph.printOut();
    
}
