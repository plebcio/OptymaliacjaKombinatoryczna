#include <vector>
#include <vector>

#include "graph.hpp"
#include "dijkstra.hpp"


using namespace std;

// znajdz najtansza sciezkie z wiercholka x do y
// zwraca jej koszt
// bool visited musi byc vectorem oddzielnym dal każdej scieżki
int dijkstra_mixed_graph(int start_V, int target_V, Graph g){
    vector <bool> visited(g.n_vertex, false);
    vector <int> path_cost(g.n_vertex, INF);

    // vector scieżek z start_V do każdego innego wierzchołka
    // kazda scieżka zaczyna się od wierzchołka start_V
    vector <vector<int>> paths(g.n_vertex, vector<int>(1, start_V) );

    

    // ustawia poczatkowy wierzchołek
    path_cost[start_V] = 0;
    int curr_vertex = start_V;
    int min_cost = INF;

    while(true){
        min_cost = INF;
        
        // znajdz wierzchołek o najniżej wartosci w path_cost
        for (int i = 0; i < g.n_vertex; i++){
            // pomija wierzchołki juz odwiedzone
            if (visited[i]) continue;
            if (path_cost[i] >= min_cost) continue;

            curr_vertex  = i;
            min_cost = path_cost[i];
        }

        visited[curr_vertex] = true;

        // aktualizuje wartości kosztów w path_cost

        // łuki
        for( auto next: g.arc_adj[curr_vertex]){
            // jesli istnieje lepsza scieżka do next, to nic nie zmieniaj
            if (g.get_arc_cost(curr_vertex, next) + path_cost[curr_vertex] >= path_cost[next]){
                continue;
            }
            
            path_cost[next] = g.get_arc_cost(curr_vertex, next) + path_cost[curr_vertex];
            // zaktualizuj scieżke do wierzchołka next
            // jako scieżka do wierzchołka curr_vertex + wierzchołek next 
            vector<int> new_vector(paths[curr_vertex]); // tworzy kopie
            new_vector.push_back(next); // dodaje do niej wierzchołek next
            paths[next] = new_vector; // przypisuje tą nową scieżkę jako najlepsza scieżke do 'next'

        } 

        // krawedzie
        for( auto next: g.edge_adj[curr_vertex]){
            if (g.get_edge_cost(curr_vertex, next) + path_cost[curr_vertex] >= path_cost[next]){    
                continue;
            }
           
            path_cost[next] = g.get_edge_cost(curr_vertex, next) + path_cost[curr_vertex];
            // zaktualizuj scieżke do wierzchołka next
            // jako scieżka do wierzchołka curr_vertex + wierzchołek next 
            vector<int> new_vector(paths[curr_vertex]); // tworzy kopie
            new_vector.push_back(next); // dodaje do niej wierzchołek next
            paths[next] = new_vector; // przypisuje tą nową scieżkę jako najlepsza scieżke do 'next'
        
        }

        // jesli znaleziono
        if (visited[target_V]){
            break;
        }
    }

    // zawraca koszt tej scieźki
    return path_cost[target_V];
} 

// Aglorytm dijkstry na grafie dla wersji potrzebnej do rozwiązania min cost max flow w algorytmie
// dla parzystych grafów
vector<int> dijkstra_for_3arc_sets(int start_V, int target_V, Graph g, vector<vector<int>> A1, vector<vector<int>> A2, vector<vector<int>> A3 , vector<vector<int>> A1_2_cost){
    vector <bool> visited(g.n_vertex, false);
    vector <int> path_cost(g.n_vertex, INF);

    // vector scieżek z start_V do każdego innego wierzchołka
    // kazda scieżka zaczyna się od wierzchołka start_V
    vector <vector<int>> paths(g.n_vertex, vector<int>(1, start_V) );

    // ustawia poczatkowy wierzchołek
    path_cost[start_V] = 0;
    int curr_vertex = start_V;
    int min_cost = INF;

    while(true){
        min_cost = INF;
        
        // znajdz wierzchołek o najniżej wartosci w path_cost
        for (int i = 0; i < g.n_vertex; i++){
            // pomija wierzchołki juz odwiedzone
            if (visited[i]) continue;
            if (path_cost[i] >= min_cost) continue;

            curr_vertex  = i;
            min_cost = path_cost[i];
        }

        visited[curr_vertex] = true;

        // aktualizuje wartości kosztów w path_cost

        // łuki w grafie 
        for( auto next: g.arc_adj[curr_vertex]){
            if (g.get_arc_cost(curr_vertex, next) + path_cost[curr_vertex] >= path_cost[next])
            {
                continue;
            }

            path_cost[next] = g.get_arc_cost(curr_vertex, next) + path_cost[curr_vertex];
            
            // zaktualizuj scieżke do wierzchołka next
            // jako scieżka do wierzchołka curr_vertex + wierzchołek next 
            vector<int> new_vector(paths[curr_vertex]); // tworzy kopie
            new_vector.push_back(next); // dodaje do niej wierzchołek next
            paths[next] = new_vector; // przypisuje tą nową scieżkę jako najlepsza scieżke do 'next'
            
        } 

        // łuki w A1
        for( auto next: A1[curr_vertex]){
            if (A1_2_cost[curr_vertex][next] + path_cost[curr_vertex] >= path_cost[next]){
                continue;
            }

            path_cost[next] = A1_2_cost[curr_vertex][next] + path_cost[curr_vertex];
            
            // zaktualizuj scieżke do wierzchołka next
            // jako scieżka do wierzchołka curr_vertex + wierzchołek next 
            vector<int> new_vector(paths[curr_vertex]); // tworzy kopie
            new_vector.push_back(next); // dodaje do niej wierzchołek next
            paths[next] = new_vector; // przypisuje tą nową scieżkę jako najlepsza scieżke do 'next'
        } 

        // łuki w A2
        for( auto next: A2[curr_vertex]){
            if (A1_2_cost[curr_vertex][next] + path_cost[curr_vertex] < path_cost[next]){
                continue;
            }

            path_cost[next] = A1_2_cost[curr_vertex][next] + path_cost[curr_vertex];
            
            // zaktualizuj scieżke do wierzchołka next
            // jako scieżka do wierzchołka curr_vertex + wierzchołek next 
            vector<int> new_vector(paths[curr_vertex]); // tworzy kopie
            new_vector.push_back(next); // dodaje do niej wierzchołek next
            paths[next] = new_vector; // przypisuje tą nową scieżkę jako najlepsza scieżke do 'next'
        } 

        // luki w A3
        for( auto next: A3[curr_vertex]){
            if ( 0 + path_cost[curr_vertex] >= path_cost[next]){
                continue;
            }

            path_cost[next] = 0 + path_cost[curr_vertex];
            
            // zaktualizuj scieżke do wierzchołka next
            // jako scieżka do wierzchołka curr_vertex + wierzchołek next 
            vector<int> new_vector(paths[curr_vertex]); // tworzy kopie
            new_vector.push_back(next); // dodaje do niej wierzchołek next
            paths[next] = new_vector; // przypisuje tą nową scieżkę jako najlepsza scieżke do 'next'
        } 


        // jesli znaleziono
        if (visited[target_V]){
            break;
        }
    }

    // zwraca scieżke do targer_V
    return paths[target_V];

}



