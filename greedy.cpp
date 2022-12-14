#include <iostream>
#include <vector>

#include "graph.hpp"
#include "dijkstra.hpp"
#include "hungarian.hpp"

using namespace std;


// zwraca graf parzysty
Graph Greedy_make_graph_even(Graph g){
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
        throw "cant have graph with odd number of uneven vertexes";
    }

    // znajdz takie połączenie wierzchołków w pary ze suma kosztów dodania takich krawedzi jest minimalny 
    // minimal cost matching problem


    // obliczam koszt dla kazdej pary wierzchołków
    vector <vector<int>> matching_cost(n_uneven, vector<int> (n_uneven, -1));
    
    for (int i = 0; i < uneven_V.size(); i++){
        for (int j = 0; j < uneven_V.size(); j++){
            if (i == j){
                matching_cost[i][j] == INF;
            } else {
                matching_cost[i][j] = dijkstra_mixed_graph(i, j, g);
            }
        }  
    }

    // znajdujemy matching o minimalnym koszcie uzywając medoty węgiersiej
    // ref https://en.wikipedia.org/wiki/Hungarian_algorithm
    // zamiast O(n!) jest O(n^4) - mozna poprawić do O(n^3) jeśli poprawimy tego loopa wyżej z dijkstrą
    vector <int> matching = hungarian(matching_cost);

    // dodaje odpowiednie połączenia w grafie
    for (int i = 0; i <matching.size(); i++){
        g.add_arc(uneven_V[i], uneven_V[matching[i]] );
        g.arc_cost[g.n_vertex*uneven_V[i] + uneven_V[matching[i]]];
    }

    if(!isEven(g)){
        cout << "matching sie nie udał\n";
        throw "bad";
    }

    return g;
}
