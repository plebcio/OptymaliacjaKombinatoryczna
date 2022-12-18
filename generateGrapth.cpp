
#include <iostream>
#include <vector>       // std::vector
#include <set>
#include <random> 
#include <cstdlib>
#include <time.h>

#include "graph.hpp"

using namespace std;

//prototypes
int vec_sum(vector<int>);
vector<vector<int>> get_shuffled_vector(vector<int> lenghts);
void print_vec(vector<int> V);
vector<int> generate_groups(int n);


// parametry stałe dla programu
#define MAX_COST 100;


// helper 
template<typename S>
auto select_random(const S &s, size_t n) {
    auto it = std::begin(s);
    
    std::advance(it,n);
    return it;
}


Graph generate_graph(int n, float egde_den, float arc_den, bool must_be_uneven){
    Graph g(n);
    
// ================== Generowanie drzewa rozpinającego  ============= //
    // Losowe drzewo rozpinające grafu jest towrzone za pomoca  algorytmu Wilsona
    // Celem jest wygenerowanie spójnego grafu, więc powstałe drzewo rozpinające jest drzewem rozpinajacym grafu pełnego
    // ref : https://en.wikipedia.org/wiki/Loop-erased_random_walk#The_uniform_spanning_tree
    
    srand(time(0));
    
    // dzielimy zbiór wierzchołków na 1 podzbiory
    set <int> S, T;
    // dodanie wyszstkich wierzchołkow do S
    for (int i = 0; i < n; i++){
        S.insert(i);
    }
    
    // losowy wierzchołek początkowy
    int currnode = rand() % n;
    S.erase(currnode);
    T.insert(currnode);

    // poki jakies wierzchołki sa poza drzewem rozpinajacym
    while(S.size() != 0){

        // losowo wybieramy jeden z wierzchołkow w S jako poczatek scieżki w błączeniu losowym (eng. random walk)
        auto r = rand() % S.size();
        int neighbor_node = *select_random(S , r);

        
        S.erase(neighbor_node);
        // sprawdzic czy neighbor_node jest w T
        if ( T.count(neighbor_node) == 0 ){

            // dodajemy krawędz
            g.add_edge(currnode, neighbor_node);
            S.erase(neighbor_node);
            T.insert(neighbor_node);
        }
        // ustawiamy nowy wierzcholek jako poczatkowy wierzcholek
        currnode = neighbor_node;
    }

// ========================= Wypełnienie grafu ===================== //
    // dodaje do grafu krawędzie i łuki az do osiągnięcia odpowiedniej gęstości 
    g.fill_edges_to_density(egde_den);
    g.fill_arc_to_density(arc_den);
// ======================== Dodanie kosztów ======================== //

    // łuki
    for (int i = 0; i < n; i++){
        for (int elem: g.arc_adj[i]){
            g.arc_cost[i*n + elem] = rand() % MAX_COST + 1;
        }
    }

    // krawędzie
    for (int i = 0; i < n; i++){
        for (int elem: g.edge_adj[i]){
            if ( g.edge_cost[i*n + elem] != -1){
                continue;
            }
            int x = rand() % MAX_COST + 1;
            g.edge_cost[i*n + elem] = x;
            g.edge_cost[elem*n + i] = x;

        }
    }


    // jesli graf musi byc nieparzysty, to dodaje dodatkowe łuki
    if (must_be_uneven){
        int i,j;
        do {
            i = rand() % g.n_vertex;
            j = rand() % g.n_vertex;
        } while (i == j);

        int x = rand() % MAX_COST + 1;
        g.edge_cost[i*n + j] = x;
        g.edge_cost[j*n + i] = x;
    }

    g.creat_prev_list();

    return g;
}


int vec_sum(vector <int> V){
    int a = 0;
    for(auto e:V){
        a +=e;
    }
    return a;
}

void print_vec(vector<int> V){
    if (V.size() == 0) {
        cout << endl;
        return;
    }
    for (auto elem: V){
        cout << elem << " ";
    }
    cout << endl;
}


vector<vector<int>> get_shuffled_vector(vector<int> lenghts){
    vector<vector<int>> out;
    int count = 0;
    for (int i = 0; i < lenghts.size(); i++){
        // i -> size of next vector
        vector <int> tmp;
        for (int j = 0; j < lenghts[i]; j++ ){
            // append the next lenghts[i] numbers to tmp
            tmp.push_back(count++);
        }
        shuffle(tmp.begin(), tmp.end(), random_device());       
        out.push_back(tmp);
    }
    return out;
}
