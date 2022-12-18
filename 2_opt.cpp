#include <iostream>
#include <vector>
#include <algorithm>


using namespace std;

// zwraca graf parzysty
// wykonuje 2 opt improvment zeby poprawić rozwiązanie dopuszczalne 
// przymuje list luków dodanych do grafu i koszty dodania łuków miedzy nimi
vector<pair<int, int>> improve_2_opt(vector<pair<int, int>> nexts, vector<vector<int>> matching_cost ){
    int n = nexts.size();

    // dla kazdej pary łuków sprawdz czy ich zamiana poprawi jakość rozwiązania
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){

            // przy tych warunakch nie ma mozliwości wykonania zmiany
            if (i == j) continue;

            pair<int, int> arc_1 = nexts[i];
            pair<int, int> arc_2 = nexts[j];
            

            int curr_cost = matching_cost[arc_1.first][arc_1.second] + matching_cost[arc_2.first][arc_2.second];
            
            int new_cost = min(matching_cost[arc_1.first][arc_2.first], matching_cost[arc_2.first][arc_1.first]) 
                           + min(matching_cost[arc_1.second][arc_2.second], matching_cost[arc_2.second][arc_1.second]);

            if (curr_cost <= new_cost) continue;

            // dodaj łuk miedzy arc_1.first i arc_2.first
            if (matching_cost[arc_1.first][arc_2.first] < matching_cost[arc_2.first][arc_1.first]){
                nexts[i] = {arc_1.first, arc_2.first};
            } else {
                nexts[i] = {arc_2.first, arc_1.first};
            }

            // analogicznie dla arc_1.second i arc_2.second
            if (matching_cost[arc_1.second][arc_2.second] < matching_cost[arc_2.second][arc_1.second]){
                nexts[j] = {arc_1.second, arc_2.second};
            } else {
                nexts[j] = {arc_2.second, arc_1.second};
            }
        }
    }

    return nexts;
}

// inny rodzaj zastosowania heurystyki lokalnego przeszukania
// zamienia pare łuków (a,b), (c,d) w forme (a, b) (a, c) (a, d)
vector<pair<int, int>> merge_pairs(vector<pair<int, int>> nexts, vector<vector<int>> matching_cost ){
    int n = nexts.size();

    // dla kazdej pary łuków sprawdz czy ich zamiana poprawi jakość rozwiązania
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){

            // przy tych warunakch nie ma mozliwości wykonania zmiany
            if (i == j) continue;

            pair<int, int> arc_1 = nexts[i];
            pair<int, int> arc_2 = nexts[j];

            int curr_cost = matching_cost[arc_1.first][arc_1.second] + matching_cost[arc_2.first][arc_2.second];

            int a = arc_1.first;
            int b = arc_1.second;
            int c = arc_2.first;
            int d = arc_2.second;

            int cost_a = min(matching_cost[a][b], matching_cost[b][a]) + min(matching_cost[a][c], matching_cost[c][a]) + min(matching_cost[a][d], matching_cost[d][a]);

            int cost_b = min(matching_cost[b][a], matching_cost[a][b]) + min(matching_cost[b][c], matching_cost[c][b]) + min(matching_cost[b][d], matching_cost[d][b]);

            int cost_c = min(matching_cost[c][a], matching_cost[a][c]) + min(matching_cost[c][b], matching_cost[b][c]) + min(matching_cost[c][d], matching_cost[d][c]);

            int cost_d = min(matching_cost[d][a], matching_cost[a][d]) + min(matching_cost[d][c], matching_cost[c][d]) + min(matching_cost[d][c], matching_cost[c][d]);

            int glob_min = min(cost_a, min(cost_b, min(cost_c, cost_d)));

            // nie wykonujemy operacji
            if (curr_cost <= glob_min) continue;

            // znajdujemy wieszchołek dla którego 'cost' jest minimalny  

            vector <int> vertices (4, 0);

            if ( cost_a == glob_min){
                vertices = {a, b, c, d};
            } 
            else if (cost_b == glob_min){
                vertices = {b, a, c, d};
            } 
            else if (cost_c == glob_min)
            {
                vertices = {c, b, a, d};
            }
            else 
            {
                vertices = {d, b, c, a};
            }

            vector<pair<int, int>> pairs;

            for(int i = 1; i < 4; i++){
                if (matching_cost[ vertices[0] ][vertices[i] ] < matching_cost[ vertices[i] ][ vertices[0] ]){
                    pairs.push_back( {vertices[0], vertices[i]} );
                }
                else {
                    pairs.push_back( {vertices[i], vertices[0]} );
                }
            }

            nexts[i] = pairs[0];
            nexts[j] = pairs[1];
            nexts.push_back(pairs[2]);
        }
    }

    return nexts;
}
