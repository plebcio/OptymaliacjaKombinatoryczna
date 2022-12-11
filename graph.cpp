#include <vector>
#include <iostream>
#include <algorithm>   
#include <stack>
#include <unordered_map>

#include "graph.hpp"

using namespace std;

void Graph::printOut(){
    std::cout << "edges :" << '\n';
    int i;
    i = 0;
    for (auto v: edge_adj){
        std::cout << i << ": ";
        for (auto elem: v){
            std::cout << elem << "(" << get_edge_cost(i, elem)<<") ";
        }
        i++;
        std::cout << '\n';
    }

    std::cout << "\narcs :" << '\n';
    for (int i = 0; i < n_vertex; i++){
        std::cout << i << ": ";
        for (auto elem: arc_adj[i]){
            std::cout << elem << "(" << get_arc_cost(i, elem)<<") ";
        }
        std::cout << '\n';
    }
    
}

void Graph::add_edge(int i, int j){
    // if (edge_adj[i].size() != 0){
        // for (auto elem: edge_adj[i]){
            // if (elem == j) return;
        // }
    // }
    // pozwalamy na wielokrotne krawedzie
    edge_adj[i].push_back(j);
    edge_adj[j].push_back(i);
}

void Graph::add_arc(int i, int j){
    // pozwalamy na wielokrotne krawedzie
    arc_adj[i].push_back(j);
}

float Graph::edge_density(){
    int max2 = n_vertex*(n_vertex-1);
    float sum = 0;
    for (auto vec: edge_adj){
        sum += vec.size();
    }
    return sum/max2;
}

float Graph::arc_density(){
    int max = n_vertex*(n_vertex-1);
    float sum = 0;
    for (auto vec: arc_adj){
        sum += vec.size();
    }
    return sum/max;
}

void Graph::fill_edges_to_density(float d){    
    while( edge_density() < d){
        int v1, v2;
        do
        {
            v1 = rand() % n_vertex;
            v2 = rand() % n_vertex;
        } while (v1 == v2);
        
        add_edge(v1, v2);
    }
}

void Graph::fill_arc_to_density(float d){
    while( arc_density() < d){
        int v1, v2;
        do
        {
            v1 = rand() % n_vertex;
            v2 = rand() % n_vertex;
        } while (v1 == v2);
        
        add_arc(v1, v2);
    }
}

void Graph::creat_prev_list(){
    for (int i = 0;  i < n_vertex; i++){
        for (auto elem: arc_adj[i]){
            arc_prev[elem].push_back(i); 
        }
    }
}

int Graph::n_arcs(){
    int sum = 0;
    for (auto vec: edge_adj){
        sum += vec.size();
    }
    return sum;
}

int Graph::n_edges(){
    int sum = 0;
    for (auto vec: edge_adj){
        sum += vec.size();
    }
    return sum/2;
}

int Graph::get_arc_cost(int i, int j){
    if (arc_cost[i*n_vertex + j] == -1){
        cout << i << " "<< j << endl;
        cout <<"cant have cost for noexistent arc(" << i << " " << j << ")\ns";
        throw "cant have cost for noexistent arc";
    }
    return arc_cost[i*n_vertex + j];
}

int Graph::get_edge_cost(int i, int j){
    if (edge_cost[i*n_vertex + j] == -1){
        cout <<"cant have cost for noexistent egde: (" << i << " " << j << ")\ns";
        throw "cant have cost for noexistent edge";
    }
    return edge_cost[i*n_vertex + j];
}

void Graph::fill_cost_x(int x){
    for (size_t i = 0; i <n_vertex; i++)
    {
        for (auto elem: arc_adj[i])
        {
            arc_cost[i*n_vertex + elem] = x;
        }
    }

    for (size_t i = 0; i <n_vertex; i++)
    {
        for (auto elem: edge_adj[i])
        {
            edge_cost[i*n_vertex + elem] = x;
        }
    }
    
}

// non member functions

bool isEven(Graph g){
    // dla kazdego wieszchołka
    for (int i = 0; i< g.n_vertex; i++){
        // czy suma wchodzacych łuków, wychodzących luków, krzawedzi jest parzysta
        int sum = g.arc_prev[i].size() + g.arc_adj[i].size() + g.edge_adj[i].size();
        if (sum%2 == 1){
            return false;
        }
    }
    return true;
}

bool isSymetric(Graph g)
{ 
    for (int i = 0; i< g.n_vertex; i++){
        // czy suma wchodzacych łuków, wychodzących luków, krzawedzi jest parzysta
        if (g.arc_prev[i].size() != g.arc_adj[i].size()){
            return false;
        }
    }
    return true;
}

int DFS(int v, vector <bool> &visited, Graph g){
    // mark this vertex as visited
    visited[v] = true;

    for (auto next: g.arc_adj[v]){
        if (!visited[next]){
            DFS(next, visited, g);  
        }
    }    

    for (auto next: g.edge_adj[v]){
        if (!visited[next]){
            DFS(next, visited, g);  
        }
    }
}

bool isConnected_from_v(int vertex, const Graph &g){
    vector<bool> l = vector<bool>(g.n_vertex, false);

    DFS(vertex, l, g);

    for (int i = 0; i < g.n_vertex; i++){
        if (l[i] == false){
            return false;
        }
    }
    return true;
}

int graph_cost(Graph g){
    int arc_sum = 0;
    int edge_sum = 0;
    for (int i = 0; i < g.n_vertex; i++){
        for (auto next: g.arc_adj[i]){
            arc_sum += g.get_arc_cost(i, next);
        }
    
        for (auto next: g.edge_adj[i]){
            edge_sum += g.get_edge_cost(i, next);
        }
    }

    // każdą krawędz policzylismy 2 razy, wiec edge_sum trzba podzieli przez 2
    return arc_sum + edge_sum/2;

}


vector<int> Euler_in_directed_from_vertex(int vertex, Graph g, int *put_cost_here){
    // wartunki na istnienie petli Eulera w grafie skierowanym
    if (!isSymetric(g)){
        throw "Not symetric";
    }
    if (!isConnected_from_v(vertex, g)){
        throw "not connected from vertex";
    }

    // algorytm hierholtza

    unordered_map<int,int> edge_count;

    // policz luki wychodzace z danego wierzcholka
    for (int i=0; i<g.arc_adj.size(); i++)
    {
        edge_count[i] = g.arc_adj[i].size();
    }

    // Maintain a stack to keep vertices
    stack<int> curr_path;
 
    // vector to store final circuit
    vector<int> circuit;

    // start from argument vertex
    curr_path.push(vertex);
    int curr_v = vertex; // Current vertex
 
    while (!curr_path.empty())
    {
        // If there's remaining edge
        if (edge_count[curr_v])
        {
            // Push the vertex
            curr_path.push(curr_v);
 
            // Find the next vertex using an edge
            int next_v = g.arc_adj[curr_v].back();
 
            // and remove that edge
            edge_count[curr_v]--;
            g.arc_adj[curr_v].pop_back();
 
            // Move to next vertex
            curr_v = next_v;
        }
 
        // back-track to find remaining circuit
        else
        {
            circuit.push_back(curr_v);
 
            // Back-tracking
            curr_v = curr_path.top();
            curr_path.pop();
        }
    }
    
    int cost = 0;

    // we've got the circuit, now print it in reverse
    for (int i=circuit.size()-1; i>=0; i--)
    {
        cost += g.arc_cost[i];
        cout << circuit[i];   
        if (i != vertex)
           cout<<" -> ";
    }    
    cout << "cost = " << cost << endl;


    *put_cost_here = cost;
    return circuit;    
}
