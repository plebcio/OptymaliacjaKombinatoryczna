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
#include "greedy.hpp"
#include "generateGrapth.hpp"

using namespace std;

int main(){
    int n = 20;
    float den = 0.1;


    for (int i =0; i < 6; i++){
        Graph g = generate_graph(5 + i, den, den, false);
        if (!isConnected_from_v(4, g)){
            cout << "failed on i = " << i << endl;
            g.printOut();
            exit(1);
        }
        cout << "made the graph\n";

        Graph greedy = Greedy_make_graph_even(g);
        cout << "greedy with cost = " <<  graph_cost(g) << "\n";
        
        Graph bruteforce = make_even_brute_force(g);
        cout << "brute with cost = " <<  graph_cost(g) << "\n";


        if (!isEven(greedy)){
            cout << "failed greedy on i = " << i << endl;
            g.printOut();
            exit(2);
        }

        if (!isEven(bruteforce)){
            cout << "failed bruteforce on i = " << i << endl;
            g.printOut();
            exit(2);
        }

    }
    cout << "didn't fail";
}
