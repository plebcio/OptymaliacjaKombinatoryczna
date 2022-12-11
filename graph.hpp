#ifndef GRAPH_HPP
#define GRAPH_HPP


#include <vector>
#include <iostream>
#include <algorithm>   

#define INF 2147483647


class Graph{
public:
    int n_vertex;
    std::vector<std::vector <int>> edge_adj;
    std::vector<std::vector <int>> arc_adj;
    std::vector<std::vector <int>> arc_prev; 
    std::vector <int> edge_cost;
    std::vector <int> arc_cost;

    Graph(int n){
        n_vertex = n;
        edge_adj  = std::vector<std::vector<int>> (n);
        arc_adj   = std::vector<std::vector<int>> (n);
        arc_prev  = std::vector<std::vector<int>> (n);
        edge_cost = std::vector <int> (n*n, -1);
        arc_cost  = std::vector <int> (n*n, -1);
    }

    void printOut();

    void add_edge(int i, int j);
    
    void add_arc(int i, int j);


    float edge_density();

    float arc_density();

    void fill_edges_to_density(float d);

    void fill_arc_to_density(float d);

    void creat_prev_list();

    int n_arcs();

    int n_edges();

    int get_arc_cost(int i, int j);

    int get_edge_cost(int i, int j);

    void fill_cost_x(int x);

};

bool isEven(Graph g);
bool isSymetric(Graph g);
int DFS(int v, std::vector <bool> &visited, Graph g);
bool isConnected_from_v(int vertex, const Graph &g);
int graph_cost(Graph g);
std::vector<int> Euler_in_directed_from_vertex(int vertex, Graph g, int *put_cost_here);


#endif