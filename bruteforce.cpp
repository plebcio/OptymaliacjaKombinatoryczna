#include <iostream>
#include <vector>       // std::vector
#include <random> 
#include <memory>
#include <unordered_map>
#include <stack>
#include <list>

#include "graph.hpp"
#include "dijkstra.hpp"
#include "min_cost_max_flow.hpp"

using namespace std;

// zwraca skierowany cykl eulera grafu G
// uzuwajac algo hierholtza

// helper do tworzenia kommbinacji
vector<vector<int>> comb(int N, int K)
{
    std::string bitmask(K, 1); // K leading 1's
    bitmask.resize(N, 0); // N-K trailing 0's
    vector<vector<int>> combinations;
    // print integers and permute bitmask
    do {
        vector <int> combination;
        for (int i = 0; i < N; ++i) // [0..N-1] integers
        {
            if (bitmask[i]) {
                combination.push_back(i);
            }
        }
        combinations.push_back(combination);

    } while (std::prev_permutation(bitmask.begin(), bitmask.end()));

    return combinations;
}


Graph make_even_brute_force(Graph g){
    if (isEven(g)){
        return  g;
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

    
    // tworzy liste par wszystkich mozliwych połączeń wierzchołków
    vector <pair<int, int>> all_possible_pairs;
    for(int i = 0; i < n_uneven; i++){
        for(int j = 0; j < n_uneven; j++){
            if (i == j) continue;
            all_possible_pairs.push_back(pair<int, int>(uneven_V[i], uneven_V[j]));
        }
    }

    // koszt połączenia każdej pary wierzchołków
    vector <vector<int>> matching_cost(g.n_vertex, vector<int> (g.n_vertex, INF));
    for (int i = 0; i < uneven_V.size(); i++){
        for (int j = 0; j < uneven_V.size(); j++){
            if (i == j){
                matching_cost[i][j] == INF;
            } else {
                list <int> path_i_j;
                matching_cost[ uneven_V[i] ][ uneven_V[j] ] = dijkstra_mixed_graph(uneven_V[i], uneven_V[j], g);
            }
        }  
    }


// ==== logika pełnego przeglądu ===

    int min_path_cost = INF;
    int min_cost_sum = INF;
    Graph min_cost_graph(0);
    
    // trzeba sprawdzic wszyskie k elementowe kombinacje od k=n'/2 do k=n'-1
    // gdzie n' = all_possible_pairs.size()
    // np: optymalnym rozwiązaniem moze byc podłączenie wszyskich nieparzystych wierzchołków z jednym wierzchołkiem 
    int n_prim = n_prim = all_possible_pairs.size();
    for (int k = n_prim/2; k < n_prim; k++){

        // kazda kombinacja to wektor indeksów do wektora all_possible_pairs
        vector<vector<int>> combiantions = comb(all_possible_pairs.size(), k);
        
        
        for (auto combination: combiantions){
            Graph tmp_graph(g);
            int cost_sum = 0;

            // dodajemy wsztstkie łuki które występują w danej kombinacji
            for (auto ind: combination){
                pair<int, int> ab = all_possible_pairs[ind];
                tmp_graph.add_arc(ab.first, ab.second);
                tmp_graph.arc_prev[ab.second].push_back(ab.first);
                tmp_graph.arc_cost[g.n_vertex*ab.first + ab.second] = matching_cost[ab.first][ab.second];   
                //cost_sum += matching_cost[ab.first][ab.second];
            }


            // // jesli koszt dla tego grafu jest wiekszy od kosztu minimalnego to mozna od razu go odrzucic
            // if (cost_sum > min_cost_sum){    
            //     continue;
            // }

            // jesli powstały graf nie jest parzysty odrzucamy tę kombinacje

            // koszt cyklu eulera przez taki graf to suma kosztów wsztstkich krawędzi i łuków
            int path_cost = graph_cost(tmp_graph);
            if (path_cost < min_path_cost){
                min_cost_sum = cost_sum; // tylko dodane łuki, bez tych orginalnie w grafie G
                min_path_cost = path_cost;
                min_cost_graph = tmp_graph;
            }
        }
    }
    
    return min_cost_graph;
}

// makes graph even using greedy aproach
Graph Greedy_make_graph_even(Graph g){
    if (isEven(g)){
        return g;
    }

    list <int> uneven_V;

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
                list <int> path_i_j;
                matching_cost[i][j] = dijkstra_mixed_graph(i, j, g);
            }
        }  
    }

    // znajdujemy matching o minimalnym koszcie uzywając medoty węgiersiej
    // ref https://en.wikipedia.org/wiki/Hungarian_algorithm
    // zamiast O(n!) jest O(n^3)
    for (int i = 0; i < uneven_V.size(); i++){
        // hungarian things TODOOOOOOO
    }

    // dodaje dobre krawedzie do grafu - jako łuki
    // return ten graf - jest even -> rozwazanie wielomianowe


}
