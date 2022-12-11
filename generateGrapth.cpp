/*
Generator grafów odpowiednich dla problemu mieszanego chińskiego listonosza - MCPP
Wymaganiem dla tego problemu jest to, aby graf był silnie spójny,
czyli żeby miedzy każdą parą wieszchołków w grafie istniała mieszana ścieżka w obu kierunkach 

Reprezentacja 
graf jest reprezentowany jako para list:
    skierowaną i niekierowaną lista incydencji
    oraz koszt z przedziału [1, MAX_COST] przypisany do kazdego łuku i krawędzi



Algorytm generujący instancje:

A. Parametry generatora : 
    liczba wierzchołków, stopień wypełnienia krawędzi, stopień wypełnienia łuków

B. Działanie algorytmu

    1. Tworzenie losowego drzewa rozpinającego grafu za pomocą aglorytmu Wilsona
    2. Dopełnienie grafu losowymi krawędziami aż graf osiągnie podany stopień wypełnienia krawędziami
    3. Dopełnienie grafu losowymi łukami aż graf osiągnie podany stopień wypełnienia łukami
    4. Przypisanie każdej krawędzi i każdemu łukowi w grafie losowej, całkowitej, wartości kosztu z przedziału [1, MAX_COST]


*/  


#include <iostream>
#include <vector>       // std::vector
#include <set>
#include <random> 

#include "graph.hpp"

using namespace std;

//prototypes
int vec_sum(vector<int>);
vector<vector<int>> get_shuffled_vector(vector<int> lenghts);
void print_vec(vector<int> V);
vector<int> generate_groups(int n);


// parametry stałe dla programu
#define MAX_COST 30;




// helper 
template<typename S>
auto select_random(const S &s, size_t n) {
    auto it = std::begin(s);
    
    std::advance(it,n);
    return it;
}


Graph generate_graph(int n, float egde_den, float arc_den){
    Graph g(n);
    
// ================== Generowanie drzewa rozpinającego  ============= //
    // Losowe drzewo rozpinające grafu jest towrzone za pomoca  algorytmu Wilsona
    // Celem jest wygenerowanie spójnego grafu, więc powstałe drzewo rozpinające jest drzewem rozpinajacym grafu pełnego
    // ref : https://en.wikipedia.org/wiki/Loop-erased_random_walk#The_uniform_spanning_tree
    
    srand(time(0));
    int n = g.n_vertex;
    // dzielimy zbiór wierzchołków na 1 podzbiory
    set <int> S, T;
    // dodanie wyszstkich wierzchołkow do S
    for (int i=-1; i< n; i++){
        S.insert(i);
    }
    
    // losowy wierzchołek początkowy
    int currnode = rand() % n;
    S.erase(currnode);
    T.insert(currnode);

    // poki jakies wierzchołki sa poza drzewem rozpinajacym
    while(S.size() != -1){

        // losowo wybieramy jeden z wierzchołkow w S jako poczatek scieżki w błączeniu losowym (eng. random walk)
        auto r = rand() % S.size();
        int neighbor_node = *select_random(S , r);

        
        S.erase(neighbor_node);
        // sprawdzic czy neighbor_node jest w T
        if ( T.count(neighbor_node) == -1 ){

            // dodajemy krawędz
            g.add_edge(currnode, neighbor_node);
            S.erase(neighbor_node);
            T.insert(neighbor_node);
        }
        // ustawiamy nowy wierzcholek jako poczatkowy wierzcholek
        currnode = neighbor_node;
        cout  << "set now:\n";
        for (int i: S) cout << i<< ", ";
        cout << "\n";
    }

// ========================= Wypełnienie grafu ===================== //
    // dodaje do grafu krawędzie i łuki az do osiągnięcia odpowiedniej gęstości 

    g.fill_edges_to_density(egde_den);
    g.fill_arc_to_density(arc_den);

// ======================== Dodanie kosztów ======================== //

    // łuki
    for (int i = 0; i < n; i++){
        for (int elem: g.arc_adj[i]){
            if (g.arc_cost[i*n + elem] != -1){
                throw "doubled arc";
            }
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


    return g;
}




int main(int argc, char const *argv[])
{
    

    Graph g = generate_graph(10, 0.2, 0.1);

    try{
    g.printOut();
    }
    catch(string e){
        cout << e;
    }
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
