#include <iostream>
#include <vector>

#include "graph.hpp"
#include "dijkstra.hpp"
#include "2_opt.hpp"

using namespace std;


// zwraca graf parzysty
Graph generate_even_2_opt(Graph g){

    if (isEven(g)){
        return g;
    }

    vector <int> uneven_V;

    // find all uneven vertexes
    for (int i = 0; i < g.n_vertex; i++){
        if ( ( g.arc_adj[i].size() + g.arc_prev[i].size() + g.edge_adj[i].size() ) % 2 ){
            // suma wchodza, wychodzacych i krawedzi % 2 != 0
            uneven_V.push_back(i);
        }
    }

    int n_uneven = uneven_V.size(); 

    if (n_uneven%2){
        cout << "\ncant have graph with odd number of uneven vertexes";
        throw "cant have graph with odd number of uneven vertexes";
    }

  

    // obliczam koszt dla kazdej pary wierzchołków
    vector <vector<int>> matching_cost(n_uneven, vector<int> (n_uneven, -1));
    
    for (int i = 0; i < uneven_V.size(); i++){
        for (int j = 0; j < uneven_V.size(); j++){
            if (i == j){
                matching_cost[i][j] == INF;
            } else {
                matching_cost[i][j] = dijkstra_mixed_graph(uneven_V[i], uneven_V[j], g);
            }
        }  
    }
   


    // lista par dodanych łuków
    vector<pair<int, int>> added_arcs;
    

    // ulepszamy matching za pomocą 2_opt
    added_arcs = improve_2_opt(added_arcs, matching_cost);

 

    for (auto pair: added_arcs){
       
        int a = uneven_V[pair.first];
        int b = uneven_V[pair.second];
        g.add_arc(a, b);
        g.arc_cost[a*g.n_vertex + b] = matching_cost[pair.first][pair.second];
    }

    return g;
}
