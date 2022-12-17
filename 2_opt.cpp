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
