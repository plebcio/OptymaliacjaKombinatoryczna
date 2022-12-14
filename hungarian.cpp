#include <vector>
#include <iostream>

#include "graph.hpp"

using namespace std;

template<typename T>
void set_row(vector<vector<T>> &mat, T val, int row_ind){
    for(int j = 0; j < mat.size(); j++){
        mat[row_ind][j] = val;
    }
}


template<typename T>
void set_col(vector<vector<T>> &mat, T val, int col_ind){
    for(int i = 0; i < mat.size(); i++){
        mat[i][col_ind] = val;
    }
}

void increment_row(vector<vector<int>> &mat, int row_ind){
    for(int j = 0; j < mat.size(); j++){
        mat[row_ind][j]++;
    }
}

void increment_col(vector<vector<int>> &mat, int col_ind){
    for(int i = 0; i < mat.size(); i++){
        mat[i][col_ind]++;
    }
}


int index_with_max_val(vector<int> v){
    int max_ind = 0;
    for (int i = 0; i < v.size(); i++){
        if (v[i] > v[max_ind]){
            max_ind = i;
        }
    }
    return max_ind;
}

// zwraca matching o minimalnym koszcie w macierzy nxn  zakłądając
// ze wartości "true" w macierzy nie da się pokryć liczba lini mniejszą niż n
// zwraca odppwiedź w następujący sposób
// przypisanie par w formie (indeks, wartość)
// [2, 4, 0, 1, 3] => (0, 2), (1, 4), (2, 0), (3, 1), (4, 3)
vector<int> find_answer(vector<vector<int>> mat){    
    vector<int> answer(mat.size(), -1);
    for (int i = 0; i < mat.size(); i++){
        for (int j = 0; j < mat.size(); j++){
            if (mat[i][j] == 0){
                // znaleziono wartość true
                answer[i] = j;
                // wykreśl pozostałe wartości true w tym wierszu i w tej kolumnie 
                set_row(mat, 1, i);
                set_col(mat, 1, j);

            }
        }
    }

    return answer;
}

// aktualizuje macierz kosztów
vector<vector<int>> step_4(vector<vector<int>> cost_mat, vector<vector<int>> lines_crossed)
{
    // znajdź najmniejsza wartość mnie pokrytą przez żadną z lini
    int min_val = INF;
    for (int i = 0; i < cost_mat.size(); i++){
        for (int j = 0; j < cost_mat.size(); j++){
            if ( lines_crossed[i][j] == 0 && cost_mat[i][j] < min_val) 
                min_val = cost_mat[i][j];
        }
    }

    // odejmij min_val od każdej wartości nie pokrytej przez żadne linie
    // dodaj min_val od każdej wartości pokrytej przez 2 linie
    for (int i = 0; i < cost_mat.size(); i++){
        for (int j = 0; j < cost_mat.size(); j++){
            if (lines_crossed[i][j] == 0){
                cost_mat[i][j] -= min_val;
            }
            else if (lines_crossed[i][j] == 2){
                cost_mat[i][j] += min_val;
            }
        }
    } 

    return cost_mat;
}


bool draw_one_line(vector<vector<int>> &cost_matrix, vector<vector<int>> &lines_drawn){
    
    int n = cost_matrix.size();

    vector<int> zeros_in_row(n, 0);
    vector<int> zeros_in_col(n, 0);

    bool found_zero = false;
   
    // liczy ile zer jest w kazdym wierszu/kolumnie
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            if (cost_matrix[i][j] != 0)
                continue;
            
            // else:
            zeros_in_row[i]++;
            zeros_in_col[j]++;
            found_zero = true;
        }
    }

    if(!found_zero){
        return found_zero;
    }

    int row_max_ind = index_with_max_val(zeros_in_row);
    int col_max_ind = index_with_max_val(zeros_in_col);

    int max_zero_count = zeros_in_row[row_max_ind] > zeros_in_col[col_max_ind] ? zeros_in_row[row_max_ind] : zeros_in_col[col_max_ind]; 

    vector<int> intersects_on_zeros_col(n,  0);
    vector<int> intersects_on_zeros_row(n,  0);

    

    // dla wierszy
    for (int i = 0; i < n; i++){
        if (zeros_in_row[i] < max_zero_count)
            continue;
        // else
        for (int j = 0; j < n; j++){
            // liczy zera w kolumnach o liczbie zer równej max_zero_count
            if (cost_matrix[i][j] != 0)
                continue;

            if (zeros_in_col[j] < max_zero_count)
                continue;
            
            // głowna logika
            intersects_on_zeros_row[i]++;
        }
    }

    // analogicznie dla kolumn
    for (int j = 0; j < n; j++){
        if (zeros_in_col[j] < max_zero_count)
            continue;
        // else
        for (int i = 0; i < n; i++){
            // liczy zera w kolumnach o liczbie zer równej max_zero_count
            if (cost_matrix[i][j] != 0)
                continue;

            if (zeros_in_row[i] < max_zero_count)
                continue;
            
            // głowna logika
            intersects_on_zeros_col[j]++;
        }
    }

    // znajdź wiersz/columne dla której intersects_on_zeros_col jest najmniejsze
    int row_min_ind = 0;
    int row_min_val = INF;
    int col_min_ind = 0;
    int col_min_val = INF;

    // wiersze
    for (int i = 0; i < n; i++){
        if (zeros_in_row[i] < max_zero_count)
            continue;

        if (intersects_on_zeros_row[i] < row_min_val){
            row_min_ind = i;
            row_min_val = intersects_on_zeros_row[i];
        }
    }

    // kolumnry
    for (int i = 0; i < n; i++){
        if (zeros_in_col[i] < max_zero_count)
            {
                continue;
            }
            
        if (intersects_on_zeros_col[i] < col_min_val){
            col_min_ind = i;
            col_min_val = intersects_on_zeros_col[i];
        }
    }


    if (row_min_val < col_min_val){
        // dodajemy linia na row_min_in
        increment_row(lines_drawn, row_min_ind);   
        // usuń zera z cost cost_matrix
        set_row(cost_matrix, 1, row_min_ind);

    } else {
        // dodajemy linia na col_min_ind
        increment_col(lines_drawn, col_min_ind);

       
        // usuń zera z cost cost_matrix
        set_col(cost_matrix, 1, col_min_ind);
    }

    return true;
    
}

// zwraca ile lini jest potrzebnych do pokrycia macierzy
int draw_lines(vector<vector<int>> cost_matrix, vector<vector<int>> &lines_drawn){
    int n = cost_matrix.size();
    int line_count = 0;

    // nigdy nie pokryjemy liczba wieksza niz n
    while(line_count < n){
        bool out = draw_one_line(cost_matrix, lines_drawn);
        
        if (!out){
            return line_count;
        }

        // else
        line_count++;
    }

    return line_count;
}

// zwraca wektor par w formie (indeks, wartość)
// np:
// [2, 4, 0, 1, 3] => (0, 2), (1, 4), (2, 0), (3, 1), (4, 3)
vector<int> hungarian(vector<vector<int>> cost_matrix){
    // kopia na potem
    vector<vector<int>> original_matrix(cost_matrix);
    int n = cost_matrix.size();

    // ojemij od każdego wiersza element o minimalnym koszcie
    vector<int> min_in_row(n, INF);
    vector<int> min_in_col(n, INF);

    // znajduje minimum dla kazdego wiersza
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            if(min_in_row[i] > cost_matrix[i][j]){
                min_in_row[i] = cost_matrix[i][j];
            }
        }
    }

    // odejmuje minima 
    for (int i = 0; i< n; i++){
        for (int j = 0; j < n; j++){
            cost_matrix[i][j] -= min_in_row[i];
        }
    }

    // znajduje minimum dla każdej kolumy wiersza
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            if(min_in_col[j] > cost_matrix[i][j]){
                min_in_col[j] = cost_matrix[i][j];
            }
        }
    }

    // odejmuje minima 
    for (int i = 0; i< n; i++){
        for (int j = 0; j < n; j++){
            cost_matrix[i][j] -= min_in_col[j];
        }
    }


// === początek kroku 3 ===
    // inicjalizacja zmiennych dla kolejnych cześci algo
    vector<vector<int>> line_count(n, vector<int>(n, 0));
    vector<vector<int>> lines_drawn(n, vector<int>(n, 0));
    // wykonuj dopóki liczba lini potrzebych do pokrycia wszystkich 0 < n
    while(1) {

        int line_count = draw_lines(cost_matrix, lines_drawn);
        
        if (line_count == n){
            // konczymy 
            return find_answer(cost_matrix);
        }
        else {
            cost_matrix = step_4(cost_matrix, lines_drawn);

        }
    }
}

// int main(){
//     vector<vector<int>> mat({
//         {13, 4, 55, 4},
//         {4, 14, 13, 4},
//         {13, 12, 2, 4}, 
//         {13, 12, 2, 4}
//     });

//     vector<int> out = hungarian(mat);

//     for (int i = 0; i< out.size(); i++){
//         cout << i << " " << out[i] << endl;
//     }
// }