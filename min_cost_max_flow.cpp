#include <vector>
#include <queue>
#include <list>

#include "graph.hpp"
#include "dijkstra.hpp"
#include "min_cost_max_flow.hpp"

using namespace std;

// helper 
// analogiczny do operatora "in" w pythonie
bool operator >> (int elem, vector<int> l){
    return (count(l.begin(), l.end(), elem) != 0);
}


// graf orginalny musi miec policzona liste poprzedników
// graf wejsciowy musi byc Even
Graph set_up_Symetric_graph(Graph g){

    // tworzymy nowy graf G' do którego dodajemy 2 nowe wieezchołki (n) i (n+1)
    // które będą potrzebne do rozwiązania max flow min cost w grafie G
    Graph g_prim(g.n_vertex + 2);
     

    // list następników powstała po przydzieleniu losowej orientacji krawedzia w G
    vector<vector<int>> A1(g_prim.n_vertex);
    // lista następników, powstała po przydzieleniu krawędzią orientacji przeciwnej niz w A1
    vector<vector<int>> A2(g_prim.n_vertex);
    
    // kopia łuków w A2
    vector<vector<int>> A3(g_prim.n_vertex);
    
    // tablica kosztu łuków w A1 i A2
    vector<vector<int>> A1_2_cost(g.n_vertex, vector<int>(g.n_vertex, 0));

    // pomocnicza tablica do iterowania przez kawedzie
    vector<vector<bool>> orinted(g.n_vertex, vector <bool> (g.n_vertex, false));

    // liczba łuków wchodzacych do wierzchołka - liczba łuków wychodzacych z wierzchołka
    vector <int> S(g.n_vertex, 0);

   
    for (int i = 0; i < g.n_vertex; i++){

        // do grafu G' dodajemu wszysstkie łuki z G z ich normalnym kosztem
        for (auto next: g.arc_adj[i]){
            g_prim.add_arc(i, next);
            g_prim.arc_cost[i*g_prim.n_vertex + next] = g.get_arc_cost(i, next);
        }

        // tworzymy zbiór A1 przez zorientowanie krawędzi oraz przypisanie im wagi orginalnych krawedzi w G
        // A2 powstaje przez nadanie krawędzią odwrotnej orientacji
        for (auto next: g.edge_adj[i]){
            if (orinted[i][next]) continue;
            
            A1[i].push_back(next);
            A2[next].push_back(i);
            A3[next].push_back(i);
            
            // dodaj koszt dla powstałch łuków
            A1_2_cost[i][next] = g.get_edge_cost(i, next);
            A1_2_cost[next][i] = g.get_edge_cost(i, next);

            // najpierw zostanie odwiedzona krawędź (i, next), wiec wystarczy zaznaczyć ze (next, i) została juz zorientowana
            orinted[next][i] = true;
        }
    }

    cout << "graf po orientacji\n";
    g_prim.printOut();
    cout << "A1:\n";
    for (int i = 0; i < A1.size(); i++){
        for (auto next: A1[i]){
            printf("%d -> %d\n", i, next);
        }
    }

    // trzeba policzyć S dla kazdego wierzchołka w G (G') uzywając łuków w G oraz łuków w A1
    for (int i = 0; i < g.n_vertex; i++){
        S[i] += g.arc_adj[i].size() + A1[i].size();

        for (auto next: g.arc_adj[i]){
            S[next] -= 1;
        }
        for (auto next: A1[i]){
            S[next] -= 1;
        }
    }

    cout << "\nS:\n";
    for (int i = 0; i< S.size(); i++){
        printf("%d : %d\n", i, S[i]);
    }

    // przepustowość dla wszystkich łuków w G', A1, A2 wynosi INF, więc nie trzeba jej brać pod uwagę
    // przepustowość łuków w A3 wynosi 2
    vector< vector<int>> A3_capacity(g.n_vertex, vector <int> (g.n_vertex, 2));

    // wszystkie wierzchołki dla których S jest dodatnie łączymy z wierzchołkiem (n + 1) - sink dla całego grafu
    // wszystkie wierzchołki dla których S jest ujemne łączymy z wierzchołkiem (n1) -  source  dla całego grafu
    // przepustowość łuków z n do każdego z wierzchołków bedzie wynosić S dla tego wierzchołka
    // analogicznie dla łuków wchcodzących do n+1 (bedzie to abs(S) )
    
    int source_val = 0;
    int sink_val = 0;
    for (int i = 0; i < g.n_vertex ; i++){
        if (S[i] == 0) continue;
        if (S[i] < 0){
            source_val -= S[i]; // bo wartości są ujemne
            g_prim.add_arc(g.n_vertex, i);
            g_prim.arc_cost[g.n_vertex*g_prim.n_vertex + i] = 0;
        }
        if (S[i] > 0){
            sink_val += S[i]; 
            g_prim.add_arc(i, g.n_vertex + 1);
            g_prim.arc_cost[i*g_prim.n_vertex + g.n_vertex+1] = 0;
        }
    }

    if (source_val - sink_val != 0){
        cout << "source val != sink val: " << source_val << " " << sink_val << endl;
        throw "bad";
    }

    // faktyczny algorytm
    
    // pętla bedzie wie wykonywała póki nie zostanie osiągniety maksyalny przepływ 
    // z wierzchołka n do n+1
    while (source_val != 0){
        // znajdź najkrótszą scieżke z n do n+1
        cout << "shortest path: ";
        vector <int> shortest_path = dijkstra_for_3arc_sets(g.n_vertex, g.n_vertex+1, g_prim, A1, A2, A3, A1_2_cost);
        
        // print the path for sanity
        for (auto elem: shortest_path){
            cout << elem << " -> ";
        }
        cout << "\n";

        // uaktualnij wartości w S - elem z indeksem 1 oraz shortest_path.size()-2

        S[shortest_path[1]] += 1; // +1 bo wartość początkowa byla ujemna
        source_val --; // potrzbe by zakończyć pętle
        
        cout << "\nS:\n";
        for (int i = 0; i< S.size(); i++){
            printf("%d : %d\n", i, S[i]);
        }

        // jeśli wyczerpano przepustowość tego łuku - usuwamy go z G'
        if (S[shortest_path[1]] == 0){
            // zamiast usuwania łuku moza ustawić jego koszt na INF
            g_prim.arc_cost[g.n_vertex*g_prim.n_vertex + shortest_path[1]] = INF;
        }


        S[shortest_path[shortest_path.size()-2]] -= 1; 
        
        cout << "\nS:\n";
        for (int i = 0; i< S.size(); i++){
            printf("%d : %d\n", i, S[i]);
        }
        
        // jeśli wyczerpano przepustowość tego łuku - usuwamy go z G'
        if (S[shortest_path[shortest_path.size()-2]] == 0){
            cout << "hgellladaws";
            // zamiast usuwania łuku moza ustawić jego koszt na INF
            g_prim.arc_cost[ shortest_path[shortest_path.size()-2] * g_prim.n_vertex + g.n_vertex+1] = INF;
        }

        
        // jesli na ścieżce leży przejscie miedzy wierzchołkami miedzy którymi jest łuk w A3
        // to scieżka przejdzie tym łukiem - zerowy koszta
        
        // prziteruj przez path, jeśli istnieje łuk w A3 odpowiedający jedenmy z przejść, zmiejsz jego przepustowość o 1
        for (int i = 0; i < shortest_path.size()-1; i++){
            int from = shortest_path[i];
            int to = shortest_path[i+1];
            printf("%d -> %d: in A3 = %d\n", from, to, to >> A3[from]);
            if(to >> A3[from]){
                
                A3_capacity[from][to]--;
                // jesli przepustowość wynosi 0, to usun te łuki
                if( A3_capacity[from][to] ==  0){
                    A3[from].erase(std::remove(A3[from].begin(), A3[from].end(), to), A3[from].end());
                }
            }
        }  
    }

    // na podstawie A3_capacity ustalamy, które krawędzie zostały dobrze zorientowane w orginalnym grafies
    // jesli A3_capacity(i, j) == 2, to łuk w G' został zorientowany poprawnie tzn. nalezy dodać łuk (j,i) do G
    // jesli A3_capacity(i, j) == 0, to łuk w G' został zorientowany niepoprawnie tzn. nalezy dodać łuk (i,j) do G
    
    // z racji ze łuki z A3 sa usuwane gdy ich przepustowosc osiegnie zero to iterujemy po łukach w A2
    // z których powstaje A3

    cout << "A3:\n";
    for (int i = 0; i<A2.size(); i++){
        for(auto next : A2[i]){
            printf(" %d-> %d: cap = %d\n", i, next, A3_capacity[i][next]);
        }
    }

    for (int i = 0; i<A2.size(); i++){
        for(auto next : A2[i]){
            if (A3_capacity[i][next] == 2){
                g.add_arc(next, i);
                g.arc_cost[next*g.n_vertex + i] = g.get_edge_cost(i, next);

            }
            else if (A3_capacity[i][next] == 0){
                g.add_arc(i, next);
                g.arc_cost[i*g.n_vertex + next] = g.get_edge_cost(i, next);
            }
            else{
                cout << "ERROR A3_capacity[" << i << "][" << next <<"] == " << A3_capacity[i][next] << endl;
                throw "error"; 
            }
        }
    }
    // usun krawedzie z g
    g.edge_adj = vector<vector<int>> (g.n_vertex, vector<int>());

    // zwróc zmodyfikowany graf g
    return g;

}